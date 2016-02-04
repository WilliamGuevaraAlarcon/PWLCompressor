#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <exception> // exception
#include <string> // for exception


// double helper functions
inline double MyMax(const double & left, const double & right){ return left > right ? left : right; }
inline double MyMin(const double & left, const double & right){ return left > right ? right : left; }
inline double MyAbs(const double & X){ return X > 0 ? X : -X; }


struct ProblemInterval
{
    unsigned int SampleSet_Start;
    unsigned int SampleSet_End;
    ProblemInterval(unsigned int Start, unsigned int End) : SampleSet_Start(Start), SampleSet_End(End) {}
    ProblemInterval() : SampleSet_Start(0), SampleSet_End(0) {}
};


struct ProblemIntervalPair
{
    ProblemInterval first;
    ProblemInterval second;
    ProblemIntervalPair(ProblemInterval A, ProblemInterval B) : first(A), second(B) {}
    ProblemIntervalPair() : first(ProblemInterval()), second(ProblemInterval()) {}
};


struct MyException : public std::exception
{
    std::string s;
    MyException(std::string ss) : s(ss) {}
    ~MyException() throw () {} // Updated
    const char* what() const throw() { return s.c_str(); }
};

#endif /* TYPEDEF_H */
