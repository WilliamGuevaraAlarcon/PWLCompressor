#ifndef SolveSimpleLinearInequalityListFunction_H
#define SolveSimpleLinearInequalityListFunction_H


#include <limits>

#include "type_defs.h" 
#include "double_vec.h" // DoubleVec, DoubleVec_IT, DoubleVec_ConstIT

class DeltaRange
{
public:
    const bool SolutionExists;
    const double MinDelta;
    const double MaxDelta;
    DeltaRange(bool, double, double);
};

DeltaRange SolveSimpleLinearInequalityListFunction(const DoubleVec & A, const DoubleVec & B, const DoubleVec & C, const double Epsilon);


#endif // SolveSimpleLinearInequalityListFunction_H
