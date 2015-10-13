#ifndef SAMPLECHARACTERISTICS_H
#define SAMPLECHARACTERISTICS_H

#include <algorithm>    // std::sort
#include <numeric>      // std::partial_sum

#include "solution_line.h"
#include "solve_simple_linear_inequality_list.h"

class SampleCharacteristics
    //    An object of this class contains various statistics of a sample.
    //    This object can then be used to calculate the delta_min, delta_max, and delta_Reg as explained in the paper
{
public:
    DoubleVec Sample;
    size_t SampleSize;
    //int TimeForSorting;
private:
    double Mean;
    double Accuracy;
    //DoubleVec IncreasingIntegers; //left away since not useful in C++
    DoubleVec CumSum_Sample;
    DoubleVec Regression_Cumsum;
    DoubleVec ScaledXTVaR;
    bool CheckFullAdmissibility;
public:
    SampleCharacteristics();
    SampleCharacteristics(DoubleVec & InputSample, double InputAccuracy, const bool CheckFullAdm);
    ~SampleCharacteristics();
    SolutionLine FindBestSolutionLine(unsigned int, unsigned int) const;
    double SampleMin() const;
    double SampleMax() const;
    double SampleMean() const;
    double MminusLm1(const unsigned int) const;
private:
    double CumSum_Sample_m1(const unsigned int) const;
    double Regression_PartialSum(const unsigned int, const unsigned int) const;
    double LocalMean(const unsigned int, const unsigned int) const;
    double CalculateRegressionDelta(const unsigned int, const unsigned int) const;
    DoubleVec IneqVectors_EpsilonPart(const unsigned int, const unsigned int) const;
    DoubleVec IneqVectors_NonEpsilonPart(const unsigned int, const unsigned int) const;
    DoubleVec IneqVectors_MiddlePart(const unsigned int, const unsigned int) const;
    unsigned int CalculateBestBisectionPoint(const DoubleVec &, const DoubleVec &, const double, const unsigned int, const unsigned int) const;
};




#endif // SAMPLECHARACTERISTICS_H
