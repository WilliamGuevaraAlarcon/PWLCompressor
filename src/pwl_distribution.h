#ifndef PIECEWISELINEARDISTRIBUTION
#define PIECEWISELINEARDISTRIBUTION

// note that C++11 also defines PWL, but here the *density* is PWL, whereas we define
// the cdf to be PWL. www.cplusplus.com/reference/random/piecewise_linear_distribution/


#include "double_vec.h"

class PiecewiseLinearDistribution
{
public:
    DoubleVec Xvalues;
    DoubleVec Yvalues;

public:
    PiecewiseLinearDistribution();
    PiecewiseLinearDistribution(const DoubleVec &, const DoubleVec &);
    virtual ~PiecewiseLinearDistribution();
    unsigned int Length() const;
    void scale(const double);
    void shift(const double);
};

#endif // PIECEWISELINEARDISTRIBUTION
