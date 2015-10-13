#include "problem_stack.h"

unsigned int Myround(double number)
{
    //return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5);
    return (unsigned int)floor(number + 0.5);
}

ProblemStack::ProblemStack(){}

ProblemStack::ProblemStack(const SampleCharacteristics & SC, const bool AtomDetection, const unsigned int AtomDetectionMinimumSampleSize,
    const double RelativeAtomDetectionThreshold, const DoubleVec EnforcedInterpolationQuantiles)
    // if AtomDetection = True AND SampleSize >= AtomDetectionMinimumSampleSize
    //      AND the number of Samplepoints equal to x is larger than AtomDetectionThreshold*Samplesize, 
    // then it is declared an atom. In this case, enforce the atom to be covered by a single ProblemInterval, 
    // such that the atom is covered by single straight vertical PWL component
    // Example: if Sample = [ 24.  ,  26.75,  27.4 ,  27.45,  30, 30, 30, 30, 30, 30, 30.15,  30.5 ,  31.45,  32.7 ,  33.8 ]
    //   PS = ProblemStack(SubsampleApproximation(Sample, AtomDetectionMinimumSampleSize = 10, RelativeAtomDetectionThreshold = 0.1)
    //   gives
    //   PS.Stack = [ProblemInterval(0,3), ProblemInterval(4,9), ProblemInterval(10,14)]
{
    const unsigned int SampleSize = SC.SampleSize;
    std::vector<unsigned int> CuttingPoints; 	// CuttingPoints = [0,SampleSize]
    CuttingPoints.push_back(0);
    CuttingPoints.push_back(SampleSize);

    for (unsigned int i = 0; i < EnforcedInterpolationQuantiles.size(); ++i)
    {
        CuttingPoints.push_back(Myround(EnforcedInterpolationQuantiles[i] * SampleSize));
    }

    //# run atom detection if conditions are given
    if (AtomDetection && (SampleSize >= AtomDetectionMinimumSampleSize))
    {
        unsigned int AtomDetectionThreshold = Myround(RelativeAtomDetectionThreshold*SampleSize);
        if (AtomDetectionThreshold < 1)
            AtomDetectionThreshold = 1;

        // Find values of atoms
        DoubleVec AtomList;
        for (unsigned int i = 0; i < SampleSize - AtomDetectionThreshold; ++i)
        {
            if (SC.Sample[i] == SC.Sample[i + AtomDetectionThreshold])
            {
                AtomList.push_back(SC.Sample[i]);
                i += AtomDetectionThreshold;
            }
        }
        AtomList = MakeUniqueAndSorted(AtomList);

        // find begin and end indices of atoms in the sample
        unsigned int SampleInd = 0;
        double AtomValue;
        for (unsigned int AtomInd = 0; AtomInd < AtomList.size(); ++AtomInd)
        {
            AtomValue = AtomList[AtomInd];
            while (SC.Sample[SampleInd] < AtomValue)
                ++SampleInd;
            CuttingPoints.push_back(SampleInd); //index of first occurence of Atom
            while ((SampleInd < SampleSize) && (SC.Sample[SampleInd] == AtomValue))
                ++SampleInd;
            CuttingPoints.push_back(SampleInd); //index of first occurence of some value bigger than Atom
        }
    }

    CuttingPoints = MakeUniqueAndSorted(CuttingPoints);

    //  create ProblemIntervals according to CuttingPoints
    // In case no atom was found, we have Stack = [ ProblemInterval(0, SampleSize - 1) ] 
    for (unsigned int i = 0; i < CuttingPoints.size() - 1; ++i)
    {
        extend(ProblemInterval(CuttingPoints[i], CuttingPoints[i + 1] - 1));
    }
}


ProblemStack::~ProblemStack(void){}

bool ProblemStack::isNotEmpty() const
{
    return (Stack.size() > 0);
}

bool ProblemStack::isEmpty() const
{
    return !isNotEmpty();
}

unsigned int ProblemStack::size() const
{
    return Stack.size();
}

ProblemInterval ProblemStack::pop()
{
    if (isEmpty())
        throw MyException("Stack is empty");
    ProblemInterval out = Stack.back();
    Stack.pop_back();
    return out;
}

void ProblemStack::extend(ProblemInterval X)
{
    Stack.push_back(X);
}

void ProblemStack::extend(ProblemIntervalVec & ProbIntVec)
//    extend with vector of ProblemInterval's.
{
    for (unsigned int i = 0; i < ProbIntVec.size(); ++i)
        extend(ProbIntVec[i]);
}
