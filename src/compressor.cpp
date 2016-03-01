#include "compressor.h"


PWLCompressor::PWLCompressor(){}

PWLCompressor::PWLCompressor(DoubleVec Sample, const double Accuracy)
{
    const DoubleVec EnforcedInterpolationQuantiles; // empty vector...
    FullConstructor(Sample, Accuracy, EnforcedInterpolationQuantiles, DEFAULT_AtomDetectionMinimumSampleSize, DEFAULT_RelativeAtomDetectionThreshold, DEFAULT_CheckFullAdmissibility, DEFAULT_ApplySmoothing);
}

PWLCompressor::PWLCompressor(DoubleVec Sample, const double Accuracy, const DoubleVec EnforcedInterpolationQuantiles)
{
    FullConstructor(Sample, Accuracy, EnforcedInterpolationQuantiles, DEFAULT_AtomDetectionMinimumSampleSize, DEFAULT_RelativeAtomDetectionThreshold, DEFAULT_CheckFullAdmissibility, DEFAULT_ApplySmoothing);
}

PWLCompressor::PWLCompressor(DoubleVec Sample, const double Accuracy, unsigned int AtomDetectionMinimumSampleSize, double RelativeAtomDetectionThreshold)
{
    const DoubleVec EnforcedInterpolationQuantiles; // empty vector...
    FullConstructor(Sample, Accuracy, EnforcedInterpolationQuantiles, AtomDetectionMinimumSampleSize, RelativeAtomDetectionThreshold, DEFAULT_CheckFullAdmissibility, DEFAULT_ApplySmoothing);
}

PWLCompressor::PWLCompressor(DoubleVec Sample, const double Accuracy, const DoubleVec EnforcedInterpolationQuantiles, unsigned int AtomDetectionMinimumSampleSize,
    double RelativeAtomDetectionThreshold, const bool CheckFullAdmissibility, const bool ApplySmoothing)
{
    FullConstructor(Sample, Accuracy, EnforcedInterpolationQuantiles, AtomDetectionMinimumSampleSize, RelativeAtomDetectionThreshold, CheckFullAdmissibility, ApplySmoothing);
}

PWLCompressor::PWLCompressor(DoubleVec Sample, const double Accuracy, const bool CheckFullAdmissibility)
{
    const DoubleVec EnforcedInterpolationQuantiles; // empty vector...
    FullConstructor(Sample, Accuracy, EnforcedInterpolationQuantiles, DEFAULT_AtomDetectionMinimumSampleSize, DEFAULT_RelativeAtomDetectionThreshold, CheckFullAdmissibility, DEFAULT_ApplySmoothing);
}

PWLCompressor::PWLCompressor(DoubleVec Sample, const double Accuracy, const bool CheckFullAdmissibility, const bool ApplySmoothing){
    const DoubleVec EnforcedInterpolationQuantiles; // empty vector...
    FullConstructor(Sample, Accuracy, EnforcedInterpolationQuantiles, DEFAULT_AtomDetectionMinimumSampleSize, DEFAULT_RelativeAtomDetectionThreshold, CheckFullAdmissibility, ApplySmoothing);

}

void PWLCompressor::FullConstructor(DoubleVec Sample, const double Accuracy, const DoubleVec EnforcedInterpolationQuantiles,
    unsigned int AtomDetectionMinimumSampleSize, double RelativeAtomDetectionThreshold, const bool CheckFullAdmissibility,
    const bool MakePWLsmoother)
{
    const bool RemoveNegativeJumps = true; //not applying this option leads to PWL functions which are not distributions. change to false only if you know what you're doing.

    SampleStats = SampleCharacteristics(Sample, Accuracy, CheckFullAdmissibility);
    ProbStack = ProblemStack(SampleStats, true, AtomDetectionMinimumSampleSize, RelativeAtomDetectionThreshold, EnforcedInterpolationQuantiles);
    SolStack = SolutionStack();

    ProblemInterval ThisProblem;
    SolutionLine ThisSolutionLine;
    ProblemIntervalPair BisectedProblem;
    ProblemIntervalVec ProblemIntervalsToAdd;
    while (ProbStack.isNotEmpty())
    {
        ThisProblem = ProbStack.pop(); // pop a problem and find the corresponding solution
        ThisSolutionLine = SampleStats.FindBestSolutionLine(ThisProblem.SampleSet_Start, ThisProblem.SampleSet_End);

        //    # if solution is acceptable, use it. otherwise bisect the problemInterval and push problem stack
        if (ThisSolutionLine.SolutionState)
            SolStack.append(ThisSolutionLine);
        else // else, bisect the interval and look for solutions in the smaller intervals
        {
            BisectedProblem = ThisSolutionLine.Bisect();
            ProbStack.extend(BisectedProblem.first);
            ProbStack.extend(BisectedProblem.second);
        }
        // if problemstack is empty, i.e., when the last problem has been removed, check for negative increments
        // if negative increments exist, either fix by making solutions steeper or pop one.
        if (ProbStack.isEmpty() && RemoveNegativeJumps)
        {
            ProblemIntervalsToAdd = SolStack.CorrectOrPopNegativeIncrements();
            ProbStack.extend(ProblemIntervalsToAdd);
        }

        // if all problems have been solved and no negative jumps exist, then smoothen solution
        if (ProbStack.isEmpty() && MakePWLsmoother)
        {
            SolStack.SmoothenSolutions();
        }

        // if an admissible solution has been found, check whether it is strictly admissible
        if (ProbStack.isEmpty() && CheckFullAdmissibility)
        {
            ProblemIntervalsToAdd = SolStack.CheckStrictAdmissibility(SampleStats, Accuracy);
            ProbStack.extend(ProblemIntervalsToAdd);
        }

    }

    Result = SolStack.GivePWL();
}


PWLCompressor::~PWLCompressor(void){}


PiecewiseLinearDistribution PWLCompressor::GetResult() const
{
    return Result;
}

DoubleVec PWLCompressor::GetSample() const
{
    return SampleStats.Sample;
}
