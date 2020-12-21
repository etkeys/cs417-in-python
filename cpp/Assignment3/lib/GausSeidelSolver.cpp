#include <stdexcept>
#include <string>

#include "GausSeidelSolver.hxx"
#include "Matrix.hxx"
#include "Utilities.hxx"

#ifndef ENABLETRACE_GAUSSEIDELSOLVER
    #ifdef TRACESOLVER
        #define ENABLETRACE_GAUSSEIDELSOLVER true
    #else
        #define ENABLETRACE_GAUSSEIDELSOLVER false
    #endif
#endif
#ifndef METHODHEADER_GAUSSEIDELSOLVER
    #define METHODHEADER_GAUSSEIDELSOLVER Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_GAUSSEIDELSOLVER)
#endif

using namespace std;

GausSeidelSolver::GausSeidelSolver(){
    METHODHEADER_GAUSSEIDELSOLVER;
}
GausSeidelSolver::GausSeidelSolver(const GausSeidelSolver& other ){
    METHODHEADER_GAUSSEIDELSOLVER;
    *this = other;
}
GausSeidelSolver::~GausSeidelSolver(){
    METHODHEADER_GAUSSEIDELSOLVER;
    Clear();
}
GausSeidelSolver& GausSeidelSolver::operator=(const GausSeidelSolver& other){
    METHODHEADER_GAUSSEIDELSOLVER;
    if (this != &other){
        JacobiSolver::operator=(other);
    }
    return *this;
}

void GausSeidelSolver::AddDataElement(Solver::DataElement param, Matrix* m){
    METHODHEADER_GAUSSEIDELSOLVER;
    switch(param){
        default:
            JacobiSolver::AddDataElement(param, m);break;
    }
}


Matrix* GausSeidelSolver::CalculateIterationResult(const Matrix* m){
    METHODHEADER_GAUSSEIDELSOLVER;
    Matrix* Dx = MatrixOperations::Multiply(_D, m);
    Matrix* result = MatrixOperations::Add(Dx, _C);
    (*_iterCount)(0,0)++;

    delete Dx;
    return result;
}

void GausSeidelSolver::Clear(){
    METHODHEADER_GAUSSEIDELSOLVER;
    delete _L;
    delete _U;
};

void GausSeidelSolver::CreateInterimMatrices(){
    METHODHEADER_GAUSSEIDELSOLVER;
    CreateLMatrix();
    CreateUMatrix();
    CreateDMatrix();
    CreateCMatrix();
}

void GausSeidelSolver::CreateCMatrix(){
    METHODHEADER_GAUSSEIDELSOLVER;
    _C = MatrixOperations::Multiply(_L, _b);
#ifdef DEBUG
    cout << "\nMatrixC: " << endl; _C->PrintPretty(cout);
#endif
#ifndef DEBUG
    delete _L; _L = nullptr;
#endif
}

void GausSeidelSolver::CreateDMatrix(){
    METHODHEADER_GAUSSEIDELSOLVER;
    Matrix* temp = MatrixOperations::Multiply(_L, _U);
    _D = MatrixOperations::MultiplyByScalar(temp, -1.0);
#ifdef DEBUG
    cout << "\nMatrixD: " << endl; _D->PrintPretty(cout);
#endif
#ifndef DEBUG
    delete _U; _U = nullptr;
#endif
    delete temp;
}

void GausSeidelSolver::CreateLMatrix(){
    METHODHEADER_GAUSSEIDELSOLVER;
    _L = Matrix::CreateLComponentFromMatrix(_A);
#ifdef DEBUG
    cout << "\nMatrixL (before inversion): " << endl; _L->PrintPretty(cout);
#endif
    _L->Invert();
#ifdef DEBUG
    cout << "\nMatrixL (after inversion): " << endl; _L->PrintPretty(cout);
#endif
}

void GausSeidelSolver::CreateUMatrix(){
    METHODHEADER_GAUSSEIDELSOLVER;
    _U = Matrix::CreateUComponentFromMatrix(_A, true);
#ifdef DEBUG
    cout << "\nMatrixU: " << endl; _U->PrintPretty(cout);
#endif
}

Matrix* GausSeidelSolver::GetDataElement(Solver::DataElement param){
    METHODHEADER_GAUSSEIDELSOLVER;
    switch(param){
        default:
            return JacobiSolver::GetDataElement(param);
    }
}

void GausSeidelSolver::PrintPrettyResults(ostream& out) const{
    METHODHEADER_GAUSSEIDELSOLVER;

    JacobiSolver::PrintPrettyResults(out, "\nGaus-Seidel Solver Results:");
}

bool GausSeidelSolver::SolveCompletely(){
    METHODHEADER_GAUSSEIDELSOLVER;
    bool result = false;
    try{
        CreateInterimMatrices();
        IterateUntilSolved(true);
        result = true;
    }catch(exception ex){
        SetFailMessage(ex);
    }
    return result;
}

void GausSeidelSolver::ValidateMatrices(){
    METHODHEADER_GAUSSEIDELSOLVER;
}
