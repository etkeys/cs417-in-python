#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>
#ifdef DEBUG
    #include <sstream>
#endif
#ifndef ENABLETRACE_GAUSSIANSOLVER
    #ifdef TRACESOLVER
        #define ENABLETRACE_GAUSSIANSOLVER true
    #else
        #define ENABLETRACE_GAUSSIANSOLVER false
    #endif
#endif

#include "GaussianSolver.hxx"
#include "Matrix.hxx"
#include "Solver.def.hxx"
#include "Utilities.hxx"

using namespace std;

GaussianSolver::GaussianSolver(const GaussianSolver& other){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_GAUSSIANSOLVER);
    *this = other;
}

GaussianSolver& GaussianSolver::operator=(const GaussianSolver& other){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_GAUSSIANSOLVER);
    if (this != &other){
        Clear();
        _matA = other._matA->DeepCopy();
        _matB = other._matB->DeepCopy();
        _opResult = other._opResult->DeepCopy();
    }
    return *this;
}

GaussianSolver::~GaussianSolver(){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_GAUSSIANSOLVER);
    Clear();
}

//Methods---------------------------------------------------------
void GaussianSolver::AddDataElement(Solver::DataElement param, Matrix* m){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_GAUSSIANSOLVER);
    switch(param){
        case Solver::DataElement::MatrixA:
            if (_matA != NULL){delete _matA;}
            _matA = m->DeepCopy(); break;

        case Solver::DataElement::Matrixb:
            if (_matB != NULL){delete _matB;}
            _matB = m->DeepCopy(); break;

        default:
            throw invalid_argument(_INVALID_DATA_ELEMENT_ADD_MSG);
    }
}

void GaussianSolver::CalculateBackSolveVector(){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_GAUSSIANSOLVER);
    if (_opResult == NULL && _matA == NULL)
        throw invalid_argument("No previous stored result and no inputs to operate on");

    if (_opResult != NULL)
        _matA = _opResult;

    if (_matA->IsSingularMatrix())
        throw invalid_argument("Matrix operand is either a singular matrix or not in upper row echilon form");

    if (!_matA->IsInUpperRowEchilonForm())
        _matA->ToUpperRowEchilonForm();

    Matrix* resultVector = new Matrix(_matA->RowCount(), 1);
#ifdef DEBUG
    stringstream ss;
    ss << "\t\tResultVector size=[" << resultVector->RowCount() << ','
        << resultVector->ColumnCount() << ']';
    PrintDebug(cerr, ss.str());
#endif
    for(int r=resultVector->RowCount()-1; r >= 0; r--)
        (*resultVector)(r,0) = CalculateResultVectorRow(resultVector, r);

    _opResult = resultVector;
}

double GaussianSolver::CalculateResultVectorRow(const Matrix* rVec, const uint targetRow) const {
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_GAUSSIANSOLVER);
    uint ir = rVec->RowCount(), ic = _matA->ColumnCount()-1;
    double result = (*_matA)(targetRow,ic);

    //From our starting position i = (targetRow, lastColumn), from that value
    //  we are going to subtract product of the result vector last row value
    //  and the column to the left value in the augmented matrix. Procceed to
    //  subtract the products of the next row up and next column to the left.
#ifdef DEBUG
    stringstream ss;
    ss << "\t\t ResultVector target row=" << targetRow;
    PrintDebug(cerr, ss.str());
#endif
    while(--ir, --ic, ir > targetRow && ic > 0){
#ifdef DEBUG
    ss.flush();
    ss << "\t\tir=" << ir << ";ic=" << ic;
    PrintDebug(cerr, ss.str());
#endif
        result -= (*_matA)(targetRow, ic) * (*rVec)(ir, 0);
    PrintDebug(cerr, "\t\tDone with row and column pair");
    }
    return result;
}

void GaussianSolver::Clear(){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_GAUSSIANSOLVER);
    delete _matA;
    delete _matB;
    delete _opResult;
}

Matrix* GaussianSolver::GetDataElement(Solver::DataElement param){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_GAUSSIANSOLVER);
    Matrix* result = NULL;
    switch(param){
        case Solver::DataElement::MatrixA:
            if (_matA != NULL)
                result = new Matrix(*_matA);
            break;
        case Solver::DataElement::Matrixb:
            if (_matB != NULL)
                result = new Matrix(*_matB);
            break;
        case Solver::DataElement::Result:
            if (_opResult != NULL)
                result = new Matrix(*_opResult);
            break;
        default:
            throw invalid_argument(_INVALID_DATA_ELEMENT_GET_MSG);
    }
    return result;
}

bool GaussianSolver::SolveCompletely(){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_GAUSSIANSOLVER);
    bool result = false;

    try{
        _matA->ToUpperRowEchilonForm();
        CalculateBackSolveVector();
        result = true;
    }catch (exception ex){
        SetFailMessage(ex);
    }

    return result;
}
