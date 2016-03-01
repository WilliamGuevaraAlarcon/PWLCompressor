#ifndef PWLCOMPRESSOR_H
#define PWLCOMPRESSOR_H



#include "double_vec.h"
#include "type_defs.h"
#include "sample_characteristics.h"
#include "problem_stack.h"
#include "solution_stack.h"

const unsigned int DEFAULT_AtomDetectionMinimumSampleSize = 1000;
const double DEFAULT_RelativeAtomDetectionThreshold = 0.005;
const bool DEFAULT_CheckFullAdmissibility = false;
const bool DEFAULT_ApplySmoothing = true;

class PWLCompressor
{
public:
    PWLCompressor();
    PWLCompressor(DoubleVec Sample, const double Accuracy);
    PWLCompressor(DoubleVec Sample, const double Accuracy, const DoubleVec EnforcedInterpolationQuantiles);
    PWLCompressor(DoubleVec Sample, const double Accuracy, unsigned int AtomDetectionMinimumSampleSize, double RelativeAtomDetectionThreshold);
    PWLCompressor(DoubleVec Sample, const double Accuracy, const bool CheckFullAdmissibility);
    PWLCompressor(DoubleVec Sample, const double Accuracy, const bool CheckFullAdmissibility, const bool ApplySmoothing);
    PWLCompressor(DoubleVec Sample, const double Accuracy, const DoubleVec EnforcedInterpolationQuantiles, unsigned int AtomDetectionMinimumSampleSize, double RelativeAtomDetectionThreshold, const bool CheckFullAdmissibility, const bool ApplySmoothing);
    ~PWLCompressor(void);
    void PrintToFile() const;
    PiecewiseLinearDistribution GetResult() const;
    DoubleVec GetSample() const;
private:
    SampleCharacteristics SampleStats;
    ProblemStack ProbStack;
    SolutionStack SolStack;
    PiecewiseLinearDistribution Result;
    void FullConstructor(DoubleVec Sample, const double Accuracy, const DoubleVec EnforcedInterpolationQuantiles, unsigned int AtomDetectionMinimumSampleSize, double RelativeAtomDetectionThreshold, const bool CheckFullAdmissibility, const bool MakePWLsmoother);
};


#endif /* PWLCOMPRESSOR_H */
