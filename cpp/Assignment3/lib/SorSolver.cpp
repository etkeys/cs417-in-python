#ifndef ENABLETRACE_SORSOLVER
    #ifdef TRACESOLVER
        #define ENABLETRACE_SORSOLVER true
    #else
        #define ENABLETRACE_SORSOLVER false
    #endif
#endif
#ifndef METHODHEADER_SORSOLVER
    #define METHODHEADER_SORSOLVER Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_SORSOLVER)
#endif

#include "Matrix.hxx"
#include "SorSolver.hxx"


SorSolver::SorSolver(const SorSolver& other){
    METHODHEADER_SORSOLVER;
    *this = other;
}
SorSolver& SorSolver::operator=(const SorSolver& other){
    METHODHEADER_SORSOLVER;
    if (this != &other){
        JacobiSolver::operator=(other);
        _L = other._L->DeepCopy();
        _U = other._U->DeepCopy();
    }
    return *this;
}

SorSolver::~SorSolver(){
    METHODHEADER_SORSOLVER;
    Clear();
}


void SorSolver::AddDataElement(Solver::DataElement param, Matrix* m){
    METHODHEADER_SORSOLVER;
    switch(param){
        case Solver::DataElement::SorOmega:
            delete _omega; _omega = m->DeepCopy();break;
        default:
            GausSeidelSolver::AddDataElement(param, m);break;
    }
}

Matrix* SorSolver::CalculateIterationResult(const Matrix* guess){
    METHODHEADER_SORSOLVER;
    Matrix* gsresult = GausSeidelSolver::CalculateIterationResult(guess);
    Matrix* balancer = MatrixOperations::MultiplyByScalar(guess, 1-(*_omega)(0,0));
    Matrix* result = MatrixOperations::MultiplyByScalar(gsresult, (*_omega)(0,0));
    (*result) = (*MatrixOperations::Add(result, balancer));

    delete balancer;
    delete gsresult;
    return result;
}

void SorSolver::Clear(){
    METHODHEADER_SORSOLVER;
    delete _omega;
}

Matrix* SorSolver::GetDataElement(Solver::DataElement param){
    METHODHEADER_SORSOLVER;
    switch(param){
        case Solver::DataElement::SorOmega:
            return _omega->DeepCopy();
        default:
            return GausSeidelSolver::GetDataElement(param);
    }
}

void SorSolver::PrintPrettyResults(std::ostream& out) const{
    METHODHEADER_SORSOLVER;
    JacobiSolver::PrintPrettyResults(out, "\nSOR Solver Results:");
}

