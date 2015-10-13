#include "solve_simple_linear_inequality_list.h"

DeltaRange::DeltaRange(bool SE, double minD, double maxDo) : SolutionExists(SE), MinDelta(minD), MaxDelta(maxDo) {}

DeltaRange SolveSimpleLinearInequalityListFunction(const DoubleVec & A, const DoubleVec & B, const DoubleVec & C, const double Epsilon)
// for vectors A, B, C of same size, solve the inequality problems
//      A[i] <= x*B[i] <= C[i] for all i
//    I.e., find the maximal and minimal x which satisfies all the inequalities above.
//    Example:
//    SolveSimpleLinearInequailityList( [1,2], [2,2], [4,3]) = (True, 1.0, 1.5)
//    SolveSimpleLinearInequailityList( [1,2,2], [-2,-2,0], [4,3,1]) = (False, 0, 0)
{
    // check size conditions
    unsigned int Length = A.size();
    if (Length == 0) throw MyException("Vectors have to be nonempty");
    if (B.size() != Length || C.size() != Length) throw MyException("Vectors must have same length");

    double UpperBound = std::numeric_limits<double>::infinity();
    double LowerBound = 0;
    double Bval;

    // define iterators
    DoubleVec_ConstIT A_IT = A.begin();
    DoubleVec_ConstIT B_IT = B.begin();
    DoubleVec_ConstIT C_IT = C.begin();

    // do the loop over elements
    for (unsigned int i = 0; i != Length; ++i)
    {
        Bval = *B_IT;
        if (Bval < 0)
        {
            LowerBound = MyMax(LowerBound, *C_IT / Bval);
            UpperBound = MyMin(UpperBound, *A_IT / Bval);
        }
        else if (Bval == 0)
        {
            // compare to epsilon, since roundoff errors do/may occur for last element with Bval == 0
            if (*A_IT > Epsilon || *C_IT < -Epsilon) // if this is satisfied then no solution exists
                return DeltaRange(false, 0, 0);
        }
        else //(Bval > 0)
        {
            LowerBound = MyMax(LowerBound, *A_IT / Bval);
            UpperBound = MyMin(UpperBound, *C_IT / Bval);
        }
        //interrupt if bounds are incompatible
        if (LowerBound > UpperBound)
            return DeltaRange(false, 0, 0);
        // increment iterators
        ++A_IT;
        ++B_IT;
        ++C_IT;
    }
    return DeltaRange(true, LowerBound, UpperBound);;
}

