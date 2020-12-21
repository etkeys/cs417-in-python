#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>
#ifdef DEBUG
    #include <sstream>
#endif

#include "GaussianSolver.hxx"
#include "Matrix.hxx"
#include "Utilities.hxx"

using namespace std;

GaussianSolver::GaussianSolver(){
    Tracer _t_(__PRETTY_FUNCTION__);
}

GaussianSolver::GaussianSolver(const GaussianSolver& other){
    Tracer _t_(__PRETTY_FUNCTION__);
    *this = other;
}

GaussianSolver& GaussianSolver::operator=(const GaussianSolver& other){
    Tracer _t_(__PRETTY_FUNCTION__);
    if (this != &other){
        Clear();
        ReplaceInputParameter(_matA, other._matA);
        ReplaceInputParameter(_matB, other._matB);
        ReplaceInputParameter(_opResult, other._opResult);
    }
    return *this;
}

GaussianSolver::~GaussianSolver(){
    Tracer _t_(__PRETTY_FUNCTION__);
    Clear();
}

//Accessors-----------------------------------------------------
string GaussianSolver::GetFailMessage() const {Tracer _t_(__PRETTY_FUNCTION__); return _failMessage;}
Matrix* GaussianSolver::GetResult() const { Tracer _t_(__PRETTY_FUNCTION__); return _opResult;}

//Methods---------------------------------------------------------
void GaussianSolver::CalculateBackSolveVector(){
    Tracer _t_(__PRETTY_FUNCTION__);
    if (_opResult == NULL && _matA == NULL)
        throw invalid_argument("No previous stored result and no inputs to operate on");

    if (_opResult != NULL)
        _matA = _opResult;

    if (_matA->IsSingularMatrix() || !(_matA->IsInUpperRowEchilonForm()))
        throw invalid_argument("Matrix operand is either a singular matrix or not in upper row echilon form");

    Matrix* resultVector = new Matrix(_opResult->RowCount(), 1);
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
    Tracer _t_(__PRETTY_FUNCTION__);
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
    Tracer _t_(__PRETTY_FUNCTION__);
    ReplaceInputParameter(_matA, NULL);
    ReplaceInputParameter(_matB, NULL);
    ReplaceInputParameter(_opResult, NULL);
}

void GaussianSolver::ConvertInputToUpperRowEchilon(){
    Tracer _t_(__PRETTY_FUNCTION__);
    if (_matA == NULL)
        throw invalid_argument("Input MatrixA has not been set.");

    _opResult = new Matrix(*_matA);

    if (!_opResult->IsInUpperRowEchilonForm())
        for(uint r=0; r < _opResult->RowCount(); r++){
            FindAndSwapRowDiagonalMax(r);
            SetRowDiagonalToOne(r);
            SetBelowDiaginalRowToZero(r);
        }
}

void GaussianSolver::FindAndSwapRowDiagonalMax(const uint currRow){
    Tracer _t_(__PRETTY_FUNCTION__);
    uint maxRow = currRow;
    for(uint nextRow = currRow + 1; nextRow < _opResult->RowCount(); nextRow++)
        //is the next row Greater than but not equal to the max row?
        if ((*_opResult)(nextRow,currRow) > (*_opResult)(maxRow,currRow) &&
            !DoubleEquals((*_opResult)(nextRow,currRow), (*_opResult)(maxRow, currRow)))
            maxRow = nextRow;

    if (maxRow != currRow)
        _opResult->SwapRows(currRow, maxRow);
}

void GaussianSolver::MultiplyRowByScalar(const uint row, const double scalar){
    Tracer _t_(__PRETTY_FUNCTION__);
    for(uint c=0; c < _opResult->ColumnCount(); c++)
        if (!DoubleEquals((*_opResult)(row, c), 0.0))
            (*_opResult)(row,c) *= scalar;
}

void GaussianSolver::ReplaceInputParameter(Matrix*& toReplace, Matrix* replaceWith){
    Tracer _t_(__PRETTY_FUNCTION__);
    if(toReplace != NULL)
        delete toReplace;

    if (replaceWith != NULL)
        toReplace = new Matrix((*replaceWith));
    else
        toReplace = NULL;
}

void GaussianSolver::SetBelowDiaginalRowToZero(const uint currRow){
    Tracer _t_(__PRETTY_FUNCTION__);
    for(uint nextRow=currRow+1; nextRow < _opResult->RowCount(); nextRow++)
        SubtractScalarRowFromRow(currRow, nextRow);
}

void GaussianSolver::SetInput(Matrix* m, GaussianSolver::InputParameter param){
    Tracer _t_(__PRETTY_FUNCTION__);
    switch(param){
        case InputParameter::MatrixA:
            ReplaceInputParameter(_matA, m); break;
            _matA = m;break;
        case InputParameter::MatrixB:
            ReplaceInputParameter(_matB, m); break;
        default:
            throw invalid_argument("Requested input parameter is not valid.");
    }
}

void GaussianSolver::SetRowDiagonalToOne(const uint row){
    Tracer _t_(__PRETTY_FUNCTION__);

    //Set the diagonal to 1 or -1
    MultiplyRowByScalar(row, (1/(*_opResult)(row,row)));

    //Make sure the diagonal is positive
    //C++11 use std::signbit()
    if (signbit((*_opResult)(row,row)))
        MultiplyRowByScalar(row, -1.0);
}

void GaussianSolver::SubtractScalarRowFromRow(const uint sourceRow, const uint targetRow){
    Tracer _t_(__PRETTY_FUNCTION__);
    double targetLeadingValue = (*_opResult)(targetRow,sourceRow);
    for(uint c=sourceRow; c < _opResult->ColumnCount(); c++)
        (*_opResult)(targetRow, c) -= (targetLeadingValue * (*_opResult)(sourceRow, c));
}

bool GaussianSolver::SolveCompletely(){
    Tracer _t_(__PRETTY_FUNCTION__);
    bool result = false;

    try{
        ConvertInputToUpperRowEchilon();
        CalculateBackSolveVector();
        result = true;
    }catch (exception ex){
        _failMessage = "Error during processing.\nwhat(): " + string(ex.what());
    }

    return result;
}

//Before calling this function be sure that inputs MatrixA
// and MatrixB have been set to non-augmented matricies.
double GaussianSolver::TwoNormOfError(){
    Tracer _t_(__PRETTY_FUNCTION__);
    double result = 0.0;
    //_inputs should have the original A, original B and
    //  the results from CalculateBackSolveVector. _inputs can have
    //  augmented A and results from CalculateBackSolveVector if A
    //  is Augmented
    if (_matA == NULL || _matB == NULL || _opResult == NULL)
        throw invalid_argument("Either input MatrixA or MatrixB is NULL or no previous result has been stored.");

    Matrix* ar = MatrixOperations::Multiply(_matA, _opResult);
    Matrix* bar = MatrixOperations::Subtract(_matB, ar);

    for(uint r = 0; r < bar->RowCount(); r++)
        result += pow((*bar)(r,0), 2);

    result = sqrt(result);

    delete ar;
    delete bar;

    return result;
}
