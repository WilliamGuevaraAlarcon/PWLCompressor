#include "solution_stack.h"

const bool VERBOSE = false;

SolutionStack::SolutionStack(void){}

SolutionStack::~SolutionStack(void){}


bool SolutionStack::isNotEmpty() const
//  returns True if the stack contains an item, False otherwise
{
    return (Stack.size() > 0);
}

bool SolutionStack::isEmpty() const
//   returns False if the stack contains an item, True otherwise
{
    return (Stack.size() == 0);
}

unsigned int SolutionStack::size() const
{
    return Stack.size();
}

SolutionLine SolutionStack::pop()
{
    if (isEmpty())
        throw MyException("Stack is empty");
    SolutionLine out = Stack.back();
    Stack.pop_back();
    return out;
}

void SolutionStack::append(SolutionLine X)
// append a solution line and ensure that items in stack are soreted
{
    Stack.push_back(X);
    std::sort(Stack.begin(), Stack.end());
}


bool SolutionStack::CheckCompletenessOfSolutionStack() const
//return true if the whole interval [0,1] is covered with adjacent solutions. false otherwise
{
    unsigned int StackLength = Stack.size();
    if (StackLength == 0)
        return false;
    else
    {
        if ((Stack[0].Segment_Line_Start_Y != 0.0) || (Stack[StackLength - 1].Segment_Line_End_Y != 1.0))
            return false;
        for (unsigned int i = 0; i < StackLength - 1; ++i)
        {
            if (Stack[i].Segment_Line_End_Y != Stack[i + 1].Segment_Line_Start_Y)
                return false;
        }
        return true;
    }
}

PiecewiseLinearDistribution SolutionStack::GivePWL() const
//# produce the PWL coordinates corresponding to the current solutionstack.
{
    if (CheckCompletenessOfSolutionStack() == false)
        throw MyException("solution not complete!");
    DoubleVec X;
    DoubleVec Y;

    const double PrecisionFactor = 1e-7; //TODO: unify all comparisons of coordinates

    for (unsigned int i = 0; i < Stack.size(); ++i)
    {
        X.push_back(Stack[i].Segment_Line_Start_X);
        X.push_back(Stack[i].Segment_Line_End_X);
        Y.push_back(Stack[i].Segment_Line_Start_Y);
        Y.push_back(Stack[i].Segment_Line_End_Y);
    }

    DoubleVec Xfixed;
    DoubleVec Yfixed;
    bool Xdifferent;
    bool Ydifferent;
    bool PointRedundant;
    double PointEpsilon;
    const unsigned int Xsize = X.size();
    Xfixed.push_back(X[0]);
    Yfixed.push_back(Y[0]);
    for (unsigned int j = 1; j < Xsize; ++j)
    {
        if (Y[j] < Yfixed.back())
            throw MyException("solution not complete!");

        PointEpsilon = PrecisionFactor*MyMax(1, MyMax(MyAbs(X[j]), MyAbs(Xfixed.back())));
        Xdifferent = (MyAbs(X[j] - Xfixed.back()) > PointEpsilon);
        Ydifferent = (Y[j] != Yfixed.back());
        // in case X[j] == Xfixed.back(), only add (X[j],Y[j]) if X[j+1]>X[j].
        // I.e., do not add(X[j], Y[j]) if X[j + 1] == X[j] == Xfixed.back()
        PointRedundant = ((!Xdifferent) && (j + 1 < Xsize) && (MyAbs(X[j + 1] - Xfixed.back()) < PointEpsilon));
        if ((!PointRedundant) && (Xdifferent || Ydifferent))
        {
            Xfixed.push_back(X[j]);
            Yfixed.push_back(Y[j]);
        }
    }

    return PiecewiseLinearDistribution(Xfixed, Yfixed);
}


ProblemIntervalVec SolutionStack::CorrectOrPopNegativeIncrements()
{
    ProblemIntervalVec NewProblemIntervalsForStack;

    SolutionLineVec::iterator Sol;
    SolutionLineVec::iterator SolAbove;
    double BoundFromLeft;
    double BoundFromRight;
    double ConnectingPoint;
    bool SolIsBisectable;
    bool SolAboveIsBisectable;
    ProblemIntervalPair BisectedProblem;

    for (Sol = Stack.begin(); (Sol != (Stack.end() - 1) && (Sol != Stack.end()));)
        //advancing iterator is put in "else" part of code. see url below.
        // iterate through stack and check which segments are not compatible
        // (Sol != Stack.end()) is necessary to catch the case where last segment is popped.
    {
        SolAbove = Sol + 1;
        if (Sol->Segment_Line_End_X > SolAbove->Segment_Line_Start_X)
        {
            BoundFromLeft = MyMax(Sol->Calculate_EndXFromDelta(Sol->Delta_LowerBound), SolAbove->Segment_Line_Start_X);
            BoundFromRight = MyMin(SolAbove->Calculate_StartXFromDelta(SolAbove->Delta_LowerBound), Sol->Segment_Line_End_X);
            //  if BoundFromLeft <= BoundFromRight then the Delta parameters of Sol and SolAbove can be adjusted such
            //  that no negative increment exists any more
            if (BoundFromLeft <= BoundFromRight)
            {
                ConnectingPoint = (BoundFromLeft + BoundFromRight) / 2.0;
                // set Delta parameter of Sol and SolAbove such that they connect at ConnectingPoint
                Sol->SetDelta(Sol->Calculate_DeltaFromEndX(ConnectingPoint));
                SolAbove->SetDelta(SolAbove->Calculate_DeltaFromStartX(ConnectingPoint));
                ++Sol; // stackoverflow.com/questions/1604588/iterate-vector-remove-certain-items-as-i-go
                //if (VERBOSE)
                //	std::cout << '\n' << "Connect Solutions to remove negative increment at y = " << Sol->Segment_Line_End_Y;
            }
            else
                //  check which solution is Bisectiable. if both, bisect bigger
            {
                SolIsBisectable = Sol->isBisectable();
                SolAboveIsBisectable = SolAbove->isBisectable();
                // bisect Sol. i.e., have to cover only cases where SolAbove would be bisected and change Sol in this case
                if (SolIsBisectable && SolAboveIsBisectable && (Sol->Size() < SolAbove->Size()))
                    ++Sol;
                if ((!SolIsBisectable) && SolAboveIsBisectable)
                    ++Sol;
                if (!SolIsBisectable && !SolAboveIsBisectable)
                    throw MyException("Adjacent solutions are incompatible, but neither is bisectable.");
                //if (VERBOSE)
                //	std::cout << '\n' << "Cannot connect Solutions. Pop solution at y = " << Sol->Segment_Line_Start_Y
                //	<< " to y = " << Sol->Segment_Line_End_Y << ". Bisect at " << (Sol->BestBisectionPoint + 1) / (double)Sol->SampleSize;

                BisectedProblem = Sol->Bisect();
                NewProblemIntervalsForStack.push_back(BisectedProblem.first);
                NewProblemIntervalsForStack.push_back(BisectedProblem.second);
                Sol = Stack.erase(Sol); //deletes element and advances iterator to next valid entry
            }
        }
        else
            ++Sol; // stackoverflow.com/questions/1604588/iterate-vector-remove-certain-items-as-i-go
    }
    return NewProblemIntervalsForStack;
}


void SolutionStack::SmoothenSolutions()
{
    const unsigned int StackLength = Stack.size();
    const double PrecisionFactor = 1e-7;

    bool CanBeConnectedBelow;
    bool CanBeConnectedAbove;
    bool IsAlreadyConnected;

    double EndPointBelow;
    double PointEpsilon;
    double ConnectingDeltaFromBelow;
    double EndPointAbove;
    double DeltaImpliedFromAbove;
    double NewDelta = 0.0;
    double SmoothingIntervalLeft;
    double SmoothingIntervalRight;

    for (unsigned int i = 0; i < StackLength; ++i)
    {
        CanBeConnectedBelow = false;
        CanBeConnectedAbove = false;
        IsAlreadyConnected = false;

        if (i > 0) // if Sol is not the first solution, check compatibility with solution below
        {
            EndPointBelow = Stack[i - 1].Segment_Line_End_X;
            PointEpsilon = PrecisionFactor*MyMax(1, MyMax(MyAbs(EndPointBelow), MyAbs(Stack[i].Segment_Line_Start_X)));
            if (Stack[i].Segment_Line_Start_X - EndPointBelow  < -PointEpsilon)
                throw MyException("#check that there is no negative increment");
            if (Stack[i].Segment_Line_Start_X - EndPointBelow <= PointEpsilon)
                IsAlreadyConnected = true;
            else
            {
                ConnectingDeltaFromBelow = Stack[i].Calculate_DeltaFromStartX(EndPointBelow);
                CanBeConnectedBelow = (ConnectingDeltaFromBelow > Stack[i].Delta_LowerBound) && (ConnectingDeltaFromBelow < Stack[i].Delta_UpperBound);
            }
        }


        if (i < StackLength - 1)// if Sol is not the last solution, check compatibility with solution above
        {
            EndPointAbove = Stack[i + 1].Segment_Line_Start_X;
            PointEpsilon = PrecisionFactor*MyMax(1, MyMax(MyAbs(EndPointAbove), abs(Stack[i].Segment_Line_End_X)));
            if (EndPointAbove - Stack[i].Segment_Line_End_X < -PointEpsilon)
                throw MyException("#check that there is no negative increment");
            if (EndPointAbove - Stack[i].Segment_Line_End_X <= PointEpsilon)
                IsAlreadyConnected = true;
            else
            {
                SmoothingIntervalLeft = MyMax(Stack[i + 1].Calculate_StartXFromDelta(Stack[i + 1].Delta_UpperBound), Stack[i].Segment_Line_End_X);
                SmoothingIntervalRight = MyMin(Stack[i].Calculate_EndXFromDelta(Stack[i].Delta_UpperBound), EndPointAbove);
                CanBeConnectedAbove = (SmoothingIntervalLeft <= SmoothingIntervalRight);
                DeltaImpliedFromAbove = Stack[i].Calculate_DeltaFromEndX((SmoothingIntervalLeft + SmoothingIntervalRight) / 2);
            }
        }

        //    #test 3 cases
        if ((!IsAlreadyConnected) && (CanBeConnectedBelow || CanBeConnectedAbove))
        {
            if (CanBeConnectedBelow && CanBeConnectedAbove)
                NewDelta = MyMin(ConnectingDeltaFromBelow, DeltaImpliedFromAbove);
            if (CanBeConnectedBelow && !CanBeConnectedAbove)
                NewDelta = ConnectingDeltaFromBelow;
            if (!CanBeConnectedBelow && CanBeConnectedAbove)
                NewDelta = DeltaImpliedFromAbove;

            //if (VERBOSE)
            //	std::cout << std::setprecision(5) << '\n'
            //	<< "Make Solution flatter at y = " << Stack[i].Segment_Line_Start_Y
            //	<< " to y = " << Stack[i].Segment_Line_End_Y
            //	<< ". Old Delta = " << Stack[i].Delta_Selected
            //	<< ". New Delta = " << NewDelta
            //	<< ". Diff = " << NewDelta - Stack[i].Delta_Selected;

            Stack[i].SetDelta(NewDelta);
        }
    }
}


ProblemIntervalVec SolutionStack::CheckStrictAdmissibility(const SampleCharacteristics & SampleStats, const double Accuracy)
// Check for each solution in the solution stack whether the solution is strictly admissible. If not bisect.
{
    ProblemIntervalVec NewProblemIntervalsForStack;

    SolutionLineVec::iterator Sol;
    ProblemIntervalPair BisectedProblem;
    double MminusLm1;

    const double SampleStats_Min = SampleStats.SampleMin();
    const double SampleStats_Max = SampleStats.SampleMax();
    const double SampleStats_Mean = SampleStats.SampleMean();

    for (Sol = Stack.begin(); Sol != Stack.end();)
        //advancing iterator is put in "else" part of code. see url below.
        // iterate through stack and check which segments are not strictly admissible
        // (Sol != Stack.end()) is necessary to catch the case where last segment is popped.
    {
        MminusLm1 = SampleStats.MminusLm1(Sol->SampleSet_Start);
        if (!(Sol->IsStrictyAdmissible(SampleStats_Min, SampleStats_Max, SampleStats_Mean, MminusLm1, Accuracy)))
        {
            BisectedProblem = Sol->Bisect();
            NewProblemIntervalsForStack.push_back(BisectedProblem.first);
            NewProblemIntervalsForStack.push_back(BisectedProblem.second);
            Sol = Stack.erase(Sol); //deletes element and advances iterator to next valid entry
        }
        else
            ++Sol; // stackoverflow.com/questions/1604588/iterate-vector-remove-certain-items-as-i-go
    }
    return NewProblemIntervalsForStack;
}