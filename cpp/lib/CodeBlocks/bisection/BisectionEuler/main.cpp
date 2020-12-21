#include <cmath>
#include <iostream>

#include "Solver.hxx"
#include "Utilities.hxx"

#ifndef METHODHEADER_MAIN
    #define METHODHEADER_MAIN Tracer _t_(__FUNCTION__,true)
#endif

using namespace std;

//Bisection args
const double ACCURACY = .001;
const double END1 = 1;
const double END2 = 5;

//Euler args
const double SAMPLINGINTERVAL = 0.5;
const double STARTINGY = -1.0;
const double TARGETX = 1.5;
//const double SAMPLINGINTERVAL = 0.1;
//const double STARTINGY = -1.0;
//const double TARGETX = .4;
//const double SAMPLINGINTERVAL = 0.1;
//const double STARTINGY = 1.0;
//const double TARGETX = 1.0;

double fxBisection(double);
double fxEuler (double, double);

void BisectionTest1(){
    METHODHEADER_MAIN;
    BisectionSolver bs;

    bs.AddDelegateElement(BisectionSolver::DelegateElement::FofX, fxBisection);
    bs.AddDataElement(BisectionSolver::DataElement::BisectionEnd1, new double(END1));
    bs.AddDataElement(BisectionSolver::DataElement::BisectionEnd2, new double(END2));
    bs.AddDataElement(BisectionSolver::DataElement::BisectionAccuracy, new double(ACCURACY));

    bs.IterateUntilSolved();

    bs.PrintPrettyResults(cout);
}

void EulerTest1(){
    METHODHEADER_MAIN;
    EulerSolver es;

    es.AddDataElement(EulerSolver::DataElement::EulerSamplingInterval, new double (SAMPLINGINTERVAL));
    es.AddDataElement(EulerSolver::DataElement::EulerStartingY, new double (STARTINGY));
    es.AddDataElement(EulerSolver::DataElement::EulerTargetX, new double (TARGETX));
    es.AddDelegateElement(EulerSolver::DelegateElement::FofX, fxEuler);

    es.IterateUntilSolved();

    es.PrintPrettyResults(cout);
}

int main(){
    METHODHEADER_MAIN;
    //TODO try removing the "Matrix" files

    BisectionTest1();
    EulerTest1();

    return 0;
}

double fxBisection (double x){
    return ((3*x)/2) - 6 - (sin(2*x)/2);
}

double fxEuler (double y, double x){
    return pow(y,2) - pow(x,2);
    //return (-2 * x) - y;
    //return x + y;
}
