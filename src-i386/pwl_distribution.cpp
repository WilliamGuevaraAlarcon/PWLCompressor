#include "pwl_distribution.h"

PiecewiseLinearDistribution::PiecewiseLinearDistribution(){}


PiecewiseLinearDistribution::PiecewiseLinearDistribution(const DoubleVec & Xin, const DoubleVec & Yin) :
Xvalues(Xin), Yvalues(Yin)
{}

PiecewiseLinearDistribution::~PiecewiseLinearDistribution(){}

unsigned int PiecewiseLinearDistribution::Length() const
{
    return Xvalues.size();
}