#ifndef SOLUTIONSTACK_H
#define SOLUTIONSTACK_H

#include <vector>
#include <algorithm>    // std::sort
//#include <iostream>
//#include <iomanip>  //std::setprecision(Precision)

#include "solution_line.h"
#include "pwl_distribution.h"
#include "problem_stack.h"
#include "sample_characteristics.h"

typedef std::vector<SolutionLine> SolutionLineVec;

class SolutionStack
{
private:
    SolutionLineVec Stack;

public:
    SolutionStack(void);
    ~SolutionStack(void);
    bool isNotEmpty() const;
    bool isEmpty() const;
    unsigned int size() const;
    SolutionLine pop();
    void append(SolutionLine);
    bool CheckCompletenessOfSolutionStack() const;
    PiecewiseLinearDistribution GivePWL() const;
    ProblemIntervalVec CorrectOrPopNegativeIncrements();
    ProblemIntervalVec CheckStrictAdmissibility(const SampleCharacteristics &, const double);
    void SmoothenSolutions();
    //void print();

};

#endif // SOLUTIONSTACK_H
