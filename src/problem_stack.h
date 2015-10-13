#ifndef PROBLEMSTACK_H
#define PROBLEMSTACK_H

#include <vector>

#include "math.h"
#include "type_defs.h" // DoubleVec, DoubleVec_IT, DoubleVec_ConstIT
#include "sample_characteristics.h"

typedef std::vector<ProblemInterval> ProblemIntervalVec;


class ProblemStack
{
private:
    ProblemIntervalVec Stack;

public:
    ProblemStack();
    ProblemStack(const SampleCharacteristics & SC, const bool AtomDetection, const unsigned int AtomDetectionMinimumSampleSize, const double RelativeAtomDetectionThreshold, const DoubleVec EnforcedInterpolationQuantiles);
    ~ProblemStack(void);
    bool isNotEmpty() const;
    bool isEmpty() const;
    unsigned int size() const;
    ProblemInterval pop();
    void extend(ProblemInterval);
    void extend(ProblemIntervalVec &);
};


void RunTestCases_ProblemStack();

#endif // PROBLEMSTACK_H
