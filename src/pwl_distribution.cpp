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

void PiecewiseLinearDistribution::scale(const double a)
{
	if (a > 0)
	{
		for (unsigned int i = 0; i < Length(); ++i)
			Xvalues[i] = a * Xvalues[i];

	} else if (a < 0)	{
		DoubleVec FormerX = Xvalues;
		DoubleVec FormerY = Yvalues;

		for (unsigned int i = 0; i < Length(); ++i)
		{
			Xvalues[i] = a * FormerX[Length() - 1 - i];
			Yvalues[i] = 1- FormerY[Length() - 1 - i];
		}
	} else {
		double X0 [] = {0.0, 0.0};
		double Y0 [] = {0.0, 1.0};
		Xvalues = DoubleVec(X0, X0 + 2);
		Yvalues = DoubleVec(Y0, Y0 + 2);
	}
}

void PiecewiseLinearDistribution::shift(const double b)
{
	for (unsigned int i = 0; i < Length(); ++i)
		{
			Xvalues[i] += b;
		}
}
