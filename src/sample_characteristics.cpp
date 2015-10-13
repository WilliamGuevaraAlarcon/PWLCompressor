#include "sample_characteristics.h"

SampleCharacteristics::SampleCharacteristics(){}


SampleCharacteristics::SampleCharacteristics(DoubleVec & InputSample, double InputAccuracy, const bool CheckFullAdm)
{
    //        calculate all statistics of the sample required for compression. is done only once.
    Accuracy = InputAccuracy;
    if (Accuracy < 0)
        throw MyException("Accuracy must be nonnegative");

    Sample = InputSample;
    CheckFullAdmissibility = CheckFullAdm;

    // measure time required for sorting
    //std::chrono::high_resolution_clock::time_point Time_Start = std::chrono::high_resolution_clock::now();
    std::sort(Sample.begin(), Sample.end()); //        Sample = np.sort(np.asarray(Sample, dtype = np.float64))
    //std::chrono::high_resolution_clock::time_point Time_End = std::chrono::high_resolution_clock::now();
    //TimeForSorting = std::chrono::duration_cast<std::chrono::milliseconds>(Time_End - Time_Start).count();

    //Calculate and Check Samplesize
    SampleSize = Sample.size();
    if (SampleSize <= 1)
        throw MyException("SampleSize must be > 1");

    //     sample regression precalculations
    //        CumSum_Sample           = np.cumsum(Sample)         # CumSum_Sample[k]     = Sample[0] + ... + Sample[k]
    CumSum_Sample.resize(SampleSize); // one should use "reserve", but I dont understand why this breaks...
    std::partial_sum(Sample.begin(), Sample.end(), CumSum_Sample.begin());

    // Mean = CumSum_Sample[SampleSize-1]/SampleSize = (Sample[0] + ... + Sample[SampleSize-1])/SampleSize
    Mean = CumSum_Sample[SampleSize - 1] / SampleSize;

    //        #scaled xtVaR:    ScaledXTVaR[i] = ( (i+1)/n*xtVaR_{(i+1)/n}(F) )    (?)
    //        ScaledXTVaR = IncreasingIntegers*Mean + Mean - CumSum_Sample
    ScaledXTVaR.resize(SampleSize);
    for (size_t idx = 0; idx < SampleSize; ++idx)
        ScaledXTVaR[idx] = (idx + 1) * Mean - CumSum_Sample[idx];

    // Regression_Cumsum = np.cumsum( (IncreasingIntegers+0.5)*Sample ) # incex starts from 0, therefore +0.5 and
    Regression_Cumsum.resize(SampleSize);
    for (size_t idx = 0; idx < SampleSize; ++idx)
        Regression_Cumsum[idx] = (idx + 0.5) * Sample[idx];
    std::partial_sum(Regression_Cumsum.begin(), Regression_Cumsum.end(), Regression_Cumsum.begin());
}

SampleCharacteristics::~SampleCharacteristics()
{
}

SolutionLine SampleCharacteristics::FindBestSolutionLine(const unsigned int SampleSet_Start, const unsigned int SampleSet_End) const
{
    if ((SampleSet_Start == SampleSet_End) || (Sample[SampleSet_Start] == Sample[SampleSet_End]))
    {
        // subsample has size one or solution line segment represents a jump part, then return jump solution
        //SolutionLine(SampleSet_Start, SampleSet_End, SampleSize, SolutionState, Mean, 
        //               Delta_LowerBound, Delta_UpperBound, Delta_Regression, IsBisectable_Value, BestBisectionPoint)
        //               AdmissibilityInequality_Left, AdmissibilityInequality_Middle, AdmissibilityInequality_Right
        DoubleVec EmptyVector;
        return SolutionLine(SampleSet_Start, SampleSet_End, SampleSize, true, Sample[SampleSet_Start], 0.0, 0.0, 0.0, false, 0u, EmptyVector, EmptyVector, EmptyVector);
    }
    else // else: subsample has size larger than one AND is not jump
    {
        const unsigned int SegmentSize = (SampleSet_End + 1 - SampleSet_Start);
        double LocMean = LocalMean(SampleSet_Start, SampleSet_End); //Mean of samples in set

        //            # construct vectors defining the inequalities to be solved
        DoubleVec EpsilonPart = IneqVectors_EpsilonPart(SampleSet_Start, SampleSet_End);
        DoubleVec NonEpsilonPart = IneqVectors_NonEpsilonPart(SampleSet_Start, SampleSet_End);
        DoubleVec Ineq_Left = NonEpsilonPart - EpsilonPart;
        DoubleVec Ineq_Right = NonEpsilonPart + EpsilonPart;
        DoubleVec Ineq_Middle = IneqVectors_MiddlePart(SampleSet_Start, SampleSet_End);

        //  Inequality list leads to Delta_LowerBound and Delta_UpperBound
        DeltaRange CalculatedDeltaRange = SolveSimpleLinearInequalityListFunction(Ineq_Left, Ineq_Middle, Ineq_Right, 1e-10*MyAbs(LocMean));

        double Delta_Regression = CalculateRegressionDelta(SampleSet_Start, SampleSet_End);

        unsigned int BestBisectionPoint = CalculateBestBisectionPoint(Ineq_Middle, NonEpsilonPart, Delta_Regression, SampleSet_Start, SegmentSize);

        // If we do not check CheckFullAdmissibility, then storing the three vectors is not necessary, and we pass empty vectors instead -> faster!
        if ((!CheckFullAdmissibility) || (!CalculatedDeltaRange.SolutionExists))
        {
            DoubleVec EmptyVector;
            Ineq_Left = EmptyVector;
            Ineq_Middle = EmptyVector;
            Ineq_Right = EmptyVector;
        }
        
        //SolutionLine(SampleSet_Start, SampleSet_End, SampleSize, SolutionState, Mean, 
        //               Delta_LowerBound, Delta_UpperBound, Delta_Regression, IsBisectable_Value, BestBisectionPoint,
        //               AdmissibilityInequality_Left, AdmissibilityInequality_Middle, AdmissibilityInequality_Right
        return SolutionLine(SampleSet_Start, SampleSet_End, SampleSize, CalculatedDeltaRange.SolutionExists, LocMean, CalculatedDeltaRange.MinDelta, 
            CalculatedDeltaRange.MaxDelta, Delta_Regression, true, BestBisectionPoint, Ineq_Left, Ineq_Middle, Ineq_Right);
    }
}

double SampleCharacteristics::CumSum_Sample_m1(const unsigned int index) const
//  returns CumSum_Sample[index-1] if index > 0. 0.0 else
{
    if (index == 0)
        return 0.0;
    else
        return CumSum_Sample[index - 1];
}

double SampleCharacteristics::Regression_PartialSum(const unsigned int SampleSet_Start, const unsigned int SampleSet_End) const
{
    if (SampleSet_Start == 0)
        return Regression_Cumsum[SampleSet_End];
    else
        return Regression_Cumsum[SampleSet_End] - Regression_Cumsum[SampleSet_Start - 1];
}

double SampleCharacteristics::LocalMean(const unsigned int SampleSet_Start, const unsigned int SampleSet_End) const
{
    return (CumSum_Sample[SampleSet_End] - CumSum_Sample_m1(SampleSet_Start)) / (SampleSet_End + 1 - SampleSet_Start);
}

double SampleCharacteristics::CalculateRegressionDelta(const unsigned int SampleSet_Start, const unsigned int SampleSet_End) const
{
    double LocMean = LocalMean(SampleSet_Start, SampleSet_End);
    double Y_Right = (SampleSet_End + 1) / (double)SampleSize;
    double Y_Left = SampleSet_Start / (double)SampleSize;
    double Regression_Delta_Part1 = -3 * LocMean*(Y_Right + Y_Left) / (Y_Right - Y_Left);
    double Regression_Delta_Part2 = 6 / (SampleSize*SampleSize*(Y_Right - Y_Left)*(Y_Right - Y_Left))*Regression_PartialSum(SampleSet_Start, SampleSet_End);
    double Delta_Regression = Regression_Delta_Part1 + Regression_Delta_Part2;
    return Delta_Regression;
}

DoubleVec SampleCharacteristics::IneqVectors_EpsilonPart(const unsigned int SampleSet_Start, const unsigned int SampleSet_End) const
//  EpsilonPart = self.Accuracy * self.ScaledXTVaR[SampleSet_Start:SampleSet_End+1]
{
    DoubleVec EpsilonPart = DoubleVec(SampleSet_End - SampleSet_Start + 1, Accuracy); // initialize with value Accuracy in each component
    for (unsigned int i = 0; i < SampleSet_End - SampleSet_Start + 1; ++i)
        EpsilonPart[i] *= ScaledXTVaR[SampleSet_Start + i];
    return EpsilonPart;
}

DoubleVec SampleCharacteristics::IneqVectors_NonEpsilonPart(const unsigned int SampleSet_Start, const unsigned int SampleSet_End) const
//  PartialSumPart   = self.CumSum_Sample[SampleSet_Start:SampleSet_End+1] - self.CumSum_Sample_m1(SampleSet_Start)
//  MeanIntegralPart = (self.IncreasingIntegers[SampleSet_Start:SampleSet_End+1] -
//	                   self.IncreasingIntegers[SampleSet_Start] + 1)*LocalMean #cumsum(np.ones(SampleSet_End - SampleSet_Start + 1))
//  NonEpsilonPart   = PartialSumPart - MeanIntegralPart
{
    DoubleVec NonEpsilonPart = DoubleVec(CumSum_Sample.begin() + SampleSet_Start, CumSum_Sample.begin() + SampleSet_End + 1);
    double CumSum_Sample_m1_Val = CumSum_Sample_m1(SampleSet_Start);
    double LocMean = LocalMean(SampleSet_Start, SampleSet_End);
    for (unsigned int i = 0; i < SampleSet_End - SampleSet_Start + 1; ++i)
        NonEpsilonPart[i] -= CumSum_Sample_m1_Val + (i + 1)*LocMean;
    return NonEpsilonPart;
}

DoubleVec SampleCharacteristics::IneqVectors_MiddlePart(const unsigned int SampleSet_Start, const unsigned int SampleSet_End) const
//  Ineq_Middle = ((self.IncreasingIntegers[SampleSet_Start:SampleSet_End+1] - self.IncreasingIntegers[SampleSet_Start] + 1)*
//                 (self.IncreasingIntegers[SampleSet_Start:SampleSet_End+1] -self.IncreasingIntegers[SampleSet_End])/SegmentSize)
{
    const unsigned int SegmentSize = (SampleSet_End + 1 - SampleSet_Start);
    const double SegmentSizeDouble = SegmentSize;
    DoubleVec Ineq_Middle = DoubleVec(SampleSet_End - SampleSet_Start + 1, 1.0 / SegmentSizeDouble);
    for (unsigned int i = 0; i < SegmentSize; ++i)
    {
        Ineq_Middle[i] *= (i + 1)*(i + 1 - SegmentSizeDouble);
    }
    return Ineq_Middle;
}

unsigned int SampleCharacteristics::CalculateBestBisectionPoint(const DoubleVec & Ineq_Middle, const DoubleVec & NonEpsilonPart,
    const double Delta_Regression, const unsigned int SampleSet_Start, const unsigned int SegmentSize) const
    // calculate best bisection point based on maximum scaled xTVaR deviation
{
    unsigned int BestBisectionPoint = 0;
    double BestBisectionVal = 0.0;
    double CurrentVal = 0.0;
    for (unsigned int i = 0; i < SegmentSize; ++i)
    {
        CurrentVal = MyAbs(Delta_Regression*Ineq_Middle[i] - NonEpsilonPart[i]);
        if (CurrentVal > BestBisectionVal)
        {
            BestBisectionVal = CurrentVal;
            BestBisectionPoint = i;
        }
    }
    return SampleSet_Start + BestBisectionPoint;
}

double SampleCharacteristics::SampleMin() const
{
    return Sample[0];
}

double SampleCharacteristics::SampleMax() const
{
    return Sample[SampleSize-1];
}

double SampleCharacteristics::SampleMean() const
{
    return Mean;
}

double SampleCharacteristics::MminusLm1(const unsigned int SampleSet_Start) const
{
    if (SampleSet_Start == 0)
        return 0.0;
    return Accuracy*ScaledXTVaR[SampleSet_Start - 1];
}