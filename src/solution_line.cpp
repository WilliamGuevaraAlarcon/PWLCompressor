#include "solution_line.h"

SolutionLine::SolutionLine(){}

SolutionLine::SolutionLine(unsigned int IN_SampleSet_Start,
    unsigned int IN_SampleSet_End,
    unsigned int IN_SampleSize,
    bool IN_SolutionState,
    double IN_Mean,
    double IN_Delta_LowerBound,
    double IN_Delta_UpperBound,
    double IN_Delta_Regression,
    bool IN_IsBisectable_Value,
    unsigned int IN_BestBisectionPoint,
    DoubleVec & IN_AdmIneq_Left,
    DoubleVec & IN_AdmIneq_Middle,
    DoubleVec & IN_AdmIneq_Right) :
    SampleSet_Start(IN_SampleSet_Start),
    SampleSet_End(IN_SampleSet_End),
    SampleSize(IN_SampleSize),
    SolutionState(IN_SolutionState),
    Mean(IN_Mean),
    Delta_LowerBound(IN_Delta_LowerBound),
    Delta_UpperBound(IN_Delta_UpperBound),
    Delta_Regression(IN_Delta_Regression),
    IsBisectable_Value(IN_IsBisectable_Value),
    BestBisectionPoint(IN_BestBisectionPoint),
    AdmissibilityInequality_Left(IN_AdmIneq_Left),
    AdmissibilityInequality_Middle(IN_AdmIneq_Middle),
    AdmissibilityInequality_Right(IN_AdmIneq_Right)
{
    if (SolutionState)
    {
        if (IN_Delta_LowerBound > IN_Delta_UpperBound)
            throw MyException("assert Delta_LowerBound <= Delta_UpperBound");
        SetDelta(Delta_Regression);
    }
}

//SolutionLine::~SolutionLine(){}

bool SolutionLine::operator< (const SolutionLine & OtherObj) const
    // provide comparison operator for sorting.
{
    return (SampleSet_Start < OtherObj.SampleSet_Start);
}

unsigned int SolutionLine::Size() const
{
    return SampleSet_End - SampleSet_Start + 1;
}

double SolutionLine::ClosestAcceptableDelta(double R2) const
{
    return MyMin(MyMax(R2, Delta_LowerBound), Delta_UpperBound);
}

void SolutionLine::SetDelta(double Delta)
{
    if (!SolutionState)     // assert self.SolutionState
        throw MyException("def SetDelta(self,Delta): assert self.SolutionState");
    Delta_Selected = ClosestAcceptableDelta(Delta);
    SetPWLCoordinates();
}

void SolutionLine::SetPWLCoordinates()
{
    Segment_Line_Start_Y = SampleSet_Start / (double)SampleSize;
    Segment_Line_End_Y = (SampleSet_End + 1) / (double)SampleSize;
    Segment_Line_Start_X = Calculate_StartXFromDelta(Delta_Selected);
    Segment_Line_End_X = Calculate_EndXFromDelta(Delta_Selected);

}
bool SolutionLine::isBisectable() const
// returns true if it can be bisected
{
    return IsBisectable_Value;
}

ProblemIntervalPair SolutionLine::Bisect() const
{
    return ProblemIntervalPair(ProblemInterval(SampleSet_Start, BestBisectionPoint), ProblemInterval(BestBisectionPoint + 1, SampleSet_End));
}

double SolutionLine::Calculate_EndXFromDelta(double Delta) const
{
    return Mean + Delta;
}

double SolutionLine::Calculate_StartXFromDelta(double Delta) const
{
    return Mean - Delta;
}

double SolutionLine::Calculate_DeltaFromEndX(double EndX) const
{
    return EndX - Mean;
}

double SolutionLine::Calculate_DeltaFromStartX(double StartX) const
{
    return Mean - StartX;
}

bool SolutionLine::IsStrictyAdmissible(const double SampleStats_Min, const double SampleStats_Max, const double SampleStats_Mean, const double MminusLm1, const double Accuracy) const
{
    // returns True if the inequality | xtVaR_a(G) - xtVaR_a(F) | <= epsilon xtVaR_a(F) is satisfied
    // for all values a between Segment_Line_Start_Y and Segment_Line_End_Y
    //const double PrecisionFactor = 1e-10;
    if (Delta_UpperBound == 0) //catch degenerate cases
        return true;

    //assert(np.all(self.Delta_Selected*self.AdmissibilityInequality_Middle - self.AdmissibilityInequality_Right < abs(self.Mean)*PrecisionFactor))
    DoubleVec MminusL = (AdmissibilityInequality_Middle * Delta_Selected) - AdmissibilityInequality_Left;
    const double M2 = Delta_Selected / Size();
    double MminusLdiff;

    // check first part of segment.Since first value is not in MminusL vector, we have to do the first part
    // separately.We have : M^0 - L^0 = MminusLm1, M^1 - L^1 = MminusL[0], MminusLdiff = MminusL[0]-MminusLm1
    MminusLdiff = MminusL[0] - MminusLm1;
    if ((MyAbs(MminusLdiff) < M2) && (MyMin(MminusLm1, MminusL[0]) < 0.25*M2))
    {
        if (!SubsegmentIsStrictlyAdmissible(MminusLm1, MminusLdiff, M2))
            return false;
    }

    // check all parts of the segment from z_s + 1 / n to z_{ s + 1 }
    for (unsigned int i = 0; i < Size() - 1; ++i)
    {
        MminusLdiff = MminusL[i + 1] - MminusL[i];
        if ((MyAbs(MminusLdiff) < M2) && (MyMin(MminusL[i], MminusL[i + 1]) < 0.25*M2))
        {
            if (!SubsegmentIsStrictlyAdmissible(MminusL[i], MminusLdiff, M2))
                return false;
        }
    }

    // check whether strict admissibility is also satisfied for threshold converging to 0. This corresponds to the
    // boundary case for the first segment. Relative error is obtained by taking the limit going to 0.
    if (Segment_Line_Start_Y == 0.0)
    {
        if (MyAbs((Segment_Line_Start_X - SampleStats_Min) / (SampleStats_Mean - SampleStats_Min)) >= Accuracy)
            return false;
    }

    // check whether strict admissibility is also satisfied for threshold converging to 1. This corresponds to the
    // boundary case for the first segment. Relative error is obtained by taking the limit going to 1.
    if (Segment_Line_End_Y == 1.0)
    {
        if (MyAbs((Segment_Line_End_X - SampleStats_Max) / (SampleStats_Max - SampleStats_Mean)) >= Accuracy)
            return false;
    }

    // if none of the checks above resulted into False then the segment satisfies strict admissibility
    return true;
}

bool SolutionLine::SubsegmentIsStrictlyAdmissible(const double M0minusL0, const double MminusLdiffValue, const double M2) const
// M0minusL0 = M ^ 0 - L ^ 0
// MminusLdiffValue = (M ^ 1 - M ^ 0) - (L ^ 1 - L ^ 0)
// M2 = M2
// Returns False if inequality L(x) <= M(x) is satisfied.True if not.
{
    const double Xstar = 0.5 - 0.5*MminusLdiffValue / M2;
    const double MinValue = M0minusL0 + Xstar*MminusLdiffValue + M2*Xstar*(Xstar - 1); // MinValue = M(X^star) - L(X^star)
    return (MinValue >= 0); //return False if MinValue < 0 else True
}
