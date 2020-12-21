#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#include "Matrix.hxx"
#include "Solver.hxx"
#include "Utilities.hxx"

#define ENABLETRACE_MAIN true
#define METHODHEADER_MAIN Tracer _t_(__FUNCTION__,ENABLETRACE_MAIN)

using namespace std;

const string INPUTFILEA = "files/T3.A.5_5.def";

double FOfX(double);
double FPrimeOfX(double);

void CreateFromFile(Matrix*& m, string filespec){
    METHODHEADER_MAIN;
    ifstream file(filespec);

    m = Matrix::CreateFromInputStream(file, Matrix::StreamType::BasicText);

    file.close();
}

void PowerSolverTest1(){
    METHODHEADER_MAIN;
    PowerSolver ps;
    Matrix *A;

    CreateFromFile(A, INPUTFILEA);
    ps.AddDataElement(PowerSolver::DataElement::MatrixA, A);

    cout << "\nMatrix A: " << endl;
    ps.GetDataElement(PowerSolver::DataElement::MatrixA)->PrintPretty(cout);

    delete A;
}

void PowerSolverTest2(){
    METHODHEADER_MAIN;
    PowerSolver ps;
    Matrix *A;

    CreateFromFile(A, INPUTFILEA);
    ps.AddDataElement(PowerSolver::DataElement::MatrixA, A);

    cout << "Starting solve..." << endl;
    ps.IterateUntilSolved();

    ps.PrintPrettyResults(cout);

    delete A;
}

void PowerSolverTest3(){
    METHODHEADER_MAIN;
    PowerSolver ps;
    Matrix *A;

    CreateFromFile(A, INPUTFILEA);
    ps.AddDataElement(PowerSolver::DataElement::MatrixA, A);

    cout << "Starting solve..." << endl;

    if (ps.SolveCompletely()){
        cout << "Solver completed successfully!";
        ps.PrintPrettyResults(cout);
    }else
        cout << "Solver failed! Cause: " << ps.GetFailMessage() << endl;

    delete A;
}

void NewtonTest1(){
    METHODHEADER_MAIN;
    NewtonSolver ns;

    ns.AddDelegateElement(NewtonSolver::DelegateElement::FofX, FOfX);
    ns.AddDelegateElement(NewtonSolver::DelegateElement::FPrimeofX, FPrimeOfX);

    double r = ns.DelegateInvoke(NewtonSolver::DelegateElement::FofX, 2);
    double rprime = ns.DelegateInvoke(NewtonSolver::DelegateElement::FPrimeofX, 2);

    cout << "\n f(x) = " << r << endl;
    cout << "\n f'(x) = " << rprime << endl;
}

void NewtonTest2(){
    METHODHEADER_MAIN;
    NewtonSolver ns;

    ns.AddDelegateElement(NewtonSolver::DelegateElement::FofX, FOfX);
    ns.AddDelegateElement(NewtonSolver::DelegateElement::FPrimeofX, FPrimeOfX);

    ns.IterateUntilSolved();

    ns.PrintPrettyResults(cout);
}

void NewtonTest3(){
    METHODHEADER_MAIN;
    NewtonSolver ns;

    ns.AddDelegateElement(NewtonSolver::DelegateElement::FofX, FOfX);
    ns.AddDelegateElement(NewtonSolver::DelegateElement::FPrimeofX, FPrimeOfX);

    if (ns.SolveCompletely())
        ns.PrintPrettyResults(cout);
    else
        cout << "\nSolver failed! Cause:\n"
                << ns.GetFailMessage() << endl;
}

int main(){
    METHODHEADER_MAIN;

    PowerSolverTest1();
    PowerSolverTest2();
    PowerSolverTest3();
    NewtonTest1();
    NewtonTest2();
    NewtonTest3();

    return 0;
}

double FOfX(double x){
    return x - exp(-1 * pow(x, 2));
}

double FPrimeOfX(double x){
    return 1 + 2 * x * exp(-1 * pow(x, 2));
}
