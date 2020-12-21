#include <cmath>
#include <iostream>

#include "lib/Solver.hxx"
#include "lib/Utilities.hxx"

#ifndef METHODHEADER_MAIN
    #define METHODHEADER_MAIN Tracer _t_(__FUNCTION__,true)
#endif

using namespace std;

//Bisection values vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
const double ACCURACY = .001;
const double LEFTBOUND = 0;
const double RIGHTBOUND = 10;

double fxBisection(double x){
    return ((3*x)/2) - 6 - ((sin(2*x))/2);
}

//Euler values vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
const double SAMPLINGINTERVAL = 0.5;
const double STARTINGY = -1;
const double TARGETX = 1.5;

double fxEuler(double y, double x){
    return pow(y,2) - pow(x,2);
}

//Make stuff happen vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void BisectionMethod(){
    METHODHEADER_MAIN;
    BisectionSolver bs;

    bs.AddDataElement(BisectionSolver::DataElement::BisectionEnd1, new double (LEFTBOUND));
    bs.AddDataElement(BisectionSolver::DataElement::BisectionEnd2, new double (RIGHTBOUND));
    bs.AddDataElement(BisectionSolver::DataElement::BisectionAccuracy, new double (ACCURACY));

    bs.AddDelegateElement(BisectionSolver::DelegateElement::FofX, fxBisection);

    if (bs.SolveCompletely()){
        cout << "\nSolver completed." << endl;
        bs.PrintPrettyResults(cout);
    }else
        cout << "\nSolver failed. Cause:\n"
            << bs.GetFailMessage() << endl;
}

void EulerMethod(){
    METHODHEADER_MAIN;
    EulerSolver es;

    es.AddDataElement(EulerSolver::DataElement::EulerSamplingInterval, new double (SAMPLINGINTERVAL));
    es.AddDataElement(EulerSolver::DataElement::EulerStartingY, new double (STARTINGY));
    es.AddDataElement(EulerSolver::DataElement::EulerTargetX, new double (TARGETX));

    es.AddDelegateElement(EulerSolver::DelegateElement::FofX, fxEuler);

    if (es.SolveCompletely()){
        cout << "\nSolver Completed." << endl;
        es.PrintPrettyResults(cout);
    }else
        cout << "\nSolver Failed. Cause: "
            << es.GetFailMessage() << endl;
}

int main(){
    METHODHEADER_MAIN;

    BisectionMethod();
    EulerMethod();

    return 0;
}




