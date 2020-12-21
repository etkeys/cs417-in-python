#include <cmath>
#include <iostream>

#include "Solver.hxx"
#include "Utilities.hxx"

#ifndef METHODHEADER_MAIN
    #define METHODHEADER_MAIN Tracer _t_(__FUNCTION__,true)
#endif

using namespace std;

double HPrimeX(double x, double y);
double HPrimeY(double x, double y);
double HSolver(double x, double y, double a, double b);

void SteepestDescentTest1(){
    METHODHEADER_MAIN;
    SteepestDescentSolver sds;

    sds.SetHPrimeXDelegate(HPrimeX);
    sds.SetHPrimeYDelegate(HPrimeY);
    sds.SetHSolverDelegate(HSolver);

    double guessx, guessy;

    //Find a way to get guessx and guessy
    guessx = guessy = 1.0;

    sds.SetGuessX(guessx);
    sds.SetGuessY(guessy);

    sds.CalculateIteration();

    double resultx, resulty;

    resultx = sds.GetCurrentX();
    resulty = sds.GetCurrentY();

    sds.PrintPrettyResults(cout);

    cout << "resultX=" << resultx << ";resultY=" << resulty << endl;
}

void SteepestDescentTest2(){
    METHODHEADER_MAIN;
    SteepestDescentSolver sds;

    sds.SetHPrimeXDelegate(HPrimeX);
    sds.SetHPrimeYDelegate(HPrimeY);
    sds.SetHSolverDelegate(HSolver);

    double guessx, guessy;

    //Find a way to get guessx and guessy
    guessx = guessy = 1.0;

    sds.SetGuessX(guessx);
    sds.SetGuessY(guessy);

    sds.IterateUntilSolved();

    double resultx, resulty;

    resultx = sds.GetCurrentX();
    resulty = sds.GetCurrentY();

    sds.PrintPrettyResults(cout);

    cout << "resultX=" << resultx << ";resultY=" << resulty << endl;
}

void SteepestDescentTest3(){
    METHODHEADER_MAIN;
    SteepestDescentSolver sds;

    sds.SetHPrimeXDelegate(HPrimeX);
    sds.SetHPrimeYDelegate(HPrimeY);
    sds.SetHSolverDelegate(HSolver);

    //Find a way to get guessx and guessy
    sds.SetGuessX(60);
    sds.SetGuessY(30);

    sds.SetDiffTolerance(0.001);

    if (sds.SolveCompletely())
        sds.PrintPrettyResults(cout);
    else
        cout << "Solver FAILED!! Cause:\n" <<
            sds.GetFailMessage() << endl;
}


int main(){
    METHODHEADER_MAIN;

    SteepestDescentTest1();
    SteepestDescentTest2();
    SteepestDescentTest3();

    return 0;
}

double HSolver(double x, double y, double a, double b){
    METHODHEADER_MAIN;
    double result = 0.0;

    //Replace forumal here
    result = result - ((y*b)+(x*b)+(y*a)+(2*a*x)-(3*b));
    result = result/(pow(b,2.0)+(2*a*b)+(2*pow(a,2.0)));
    return result;
}

double HPrimeX(double x, double y){
    METHODHEADER_MAIN;
    double result = 0.0;

    //Replace forumal to value of the derivative with respect to x
    result = y+(2.0*x);
    return result;
}

double HPrimeY(double x, double y){
    METHODHEADER_MAIN;
    double result = 0.0;

    result = y + x - 3;
    return result;
}
