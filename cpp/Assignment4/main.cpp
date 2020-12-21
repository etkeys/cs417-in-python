#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#include "lib/Matrix.hxx"
#include "lib/Solver.hxx"
#include "lib/Utilities.hxx"

#ifndef METHODHEADER_MAIN
    #define METHODHEADER_MAIN Tracer _t_(__FUNCTION__,true)
#endif

using namespace std;

const string INPUTFILEA = "files/A.def";

double FOfX(double);
double FPrimeOfX(double);

void CreateFromFile(Matrix*& m, string filespec){
    METHODHEADER_MAIN;
    ifstream file(filespec);

    m = Matrix::CreateFromInputStream(file, Matrix::StreamType::BasicText);

    file.close();
}

void DoNewtonSolver(){
    METHODHEADER_MAIN;
    NewtonSolver ns;

    ns.AddDelegateElement(NewtonSolver::DelegateElement::FofX, FOfX);
    ns.AddDelegateElement(NewtonSolver::DelegateElement::FPrimeofX, FPrimeOfX);

    if (ns.SolveCompletely())
        ns.PrintPrettyResults(cout);
    else
        cout << "\nSolver failed! Cause: "
            << ns.GetFailMessage() << endl;
}


void DoPowerSolver(){
    METHODHEADER_MAIN;
    PowerSolver ps;
    Matrix *A;

    CreateFromFile(A, INPUTFILEA);

    ps.AddDataElement(PowerSolver::DataElement::MatrixA, A);

    if (ps.SolveCompletely())
        ps.PrintPrettyResults(cout);
    else
        cout << "\nSolver failed! Cause:\n"
            << ps.GetFailMessage() << endl;

}

int main(){
    METHODHEADER_MAIN;

    DoPowerSolver();
    DoNewtonSolver();

    return 0;
}

double FOfX(double x){
    return x - exp(-1 * pow(x, 2));
}

double FPrimeOfX(double x){
    return 1 + 2 * x * exp(-1 * pow(x, 2));
}
