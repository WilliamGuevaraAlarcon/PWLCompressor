#ifndef SOLUTIONLINE_H
#define SOLUTIONLINE_H

#include "type_defs.h" // ProblemIntervalPair
#include "double_vec.h"

class SolutionLine
{
public:
    unsigned int SampleSet_Start;
    unsigned int SampleSet_End;
    unsigned int SampleSize;
    bool SolutionState;
    double Mean;          //local mean
    double Delta_LowerBound;
    double Delta_UpperBound;
    double Delta_Regression;
    bool IsBisectable_Value;
    unsigned int BestBisectionPoint;

    double Delta_Selected;
    double Segment_Line_Start_Y;
    double Segment_Line_End_Y;
    double Segment_Line_Start_X;
    double Segment_Line_End_X;

    DoubleVec AdmissibilityInequality_Left;
    DoubleVec AdmissibilityInequality_Middle;
    DoubleVec AdmissibilityInequality_Right;


public:
    SolutionLine();
    SolutionLine(unsigned int IN_SampleSet_Start,
        unsigned int IN_SampleSet_End,
        unsigned int IN_SampleSize,
        bool IN_SolutionState,
        double IN_Mean,
        double IN_Delta_LowerBound,
        double IN_Delta_UpperBound,
        double IN_Delta_Regression,
        bool IN_IsBisectable_Value,
        unsigned int IN_BestBisectionPoint,
        DoubleVec & IN_AdmIneq_Left,
        DoubleVec & IN_AdmIneq_Middle,
        DoubleVec & IN_AdmIneq_Right
        );
    bool operator < (const SolutionLine &) const;
    void SetDelta(double);
    unsigned int Size() const;
    bool IsStrictyAdmissible(const double, const double, const double, const double, const double) const;

private:
    double ClosestAcceptableDelta(double) const;
    void SetPWLCoordinates();
    bool SubsegmentIsStrictlyAdmissible(const double, const double, const double) const;

public:
    bool isBisectable() const;
    ProblemIntervalPair Bisect() const;
    double Calculate_EndXFromDelta(double) const;
    double Calculate_StartXFromDelta(double) const;
    double Calculate_DeltaFromEndX(double) const;
    double Calculate_DeltaFromStartX(double) const;


};

#endif // SOLUTIONLINE_H
