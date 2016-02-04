#include "double_vec.h"


void operator*=(DoubleVec & obj1, const double multiplicator)
{
    for (unsigned int i = 0; i < obj1.size(); ++i)
        obj1[i] *= multiplicator;
}

DoubleVec operator*(const DoubleVec & obj1, const double multiplicator)
{
    //assert size...
    DoubleVec out = DoubleVec(obj1);
    for (unsigned int i = 0; i < obj1.size(); ++i)
        out[i] *= multiplicator;
    return out;
}

DoubleVec operator*(const DoubleVec & obj1, const DoubleVec & obj2)
{
    //assert size...
    DoubleVec out = DoubleVec(obj1);
    for (unsigned int i = 0; i < obj1.size(); ++i)
        out[i] *= obj2[i];
    return out;
}

DoubleVec operator+(const DoubleVec & obj1, const DoubleVec & obj2)
{
    //assert size...
    DoubleVec out = DoubleVec(obj1);
    for (unsigned int i = 0; i < obj1.size(); ++i)
        out[i] += obj2[i];
    return out;
}

DoubleVec operator-(const DoubleVec & obj1, const DoubleVec & obj2)
{
    //assert size...
    DoubleVec out = DoubleVec(obj1);
    for (unsigned int i = 0; i < obj1.size(); ++i)
        out[i] -= obj2[i];
    return out;
}

DoubleVec MakeUniqueAndSorted(DoubleVec V)
// can also be implemented using set & sort, or sort + unique, see 
// http://stackoverflow.com/questions/1041620/. however, in order not to use
// C++11 features, and no set ( to keep the numbers of imported libraries low)
// I implement it manually. old versions of the code implemented the set variant
{
    DoubleVec out;
    if (V.size() == 0)
        return out;
    std::sort(V.begin(), V.end());
    double lastvalue = V[0];
    out.push_back(lastvalue);
    for (unsigned int i = 1; i < V.size(); ++i)
    {
        if (V[i] > lastvalue)
        {
            lastvalue = V[i];
            out.push_back(lastvalue);
        }
    }
    return out;
}

std::vector<unsigned int> MakeUniqueAndSorted(std::vector<unsigned int> V)
// reimplemented to avoid templates
{
    std::vector<unsigned int> out;
    if (V.size() == 0)
        return out;
    std::sort(V.begin(), V.end());
    unsigned int lastvalue = V[0];
    out.push_back(lastvalue);
    for (unsigned int i = 1; i < V.size(); ++i)
    {
        if (V[i] > lastvalue)
        {
            lastvalue = V[i];
            out.push_back(lastvalue);
        }
    }
    return out;
}