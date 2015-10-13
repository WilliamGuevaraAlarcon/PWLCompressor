#ifndef	DOUBLEVEC_H
#define DOUBLEVEC_H

#include <vector>
//#include <set>
#include <algorithm>    // std::sort

// type definitions
typedef std::vector<double> DoubleVec;
typedef std::vector<double>::iterator DoubleVec_IT;
typedef std::vector<double>::const_iterator DoubleVec_ConstIT;
typedef std::vector<unsigned int> UIntVec;

//DoubleVec helpers
void operator*=(DoubleVec & obj1, const double multiplicator);
DoubleVec operator*(const DoubleVec & obj1, const double multiplicator);
DoubleVec operator*(const DoubleVec & obj1, const DoubleVec & obj2);
DoubleVec operator+(const DoubleVec & obj1, const DoubleVec & obj2);
DoubleVec operator-(const DoubleVec & obj1, const DoubleVec & obj2);
DoubleVec MakeUniqueAndSorted(DoubleVec V);
std::vector<unsigned int> MakeUniqueAndSorted(std::vector<unsigned int> V);
//void PrintDoubleVec(const DoubleVec & X);


#endif /* DOUBLEVEC_H */
