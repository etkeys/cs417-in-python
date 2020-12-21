#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>

#include "../../Matrix/Matrix/Matrix.hpp"
#include "GaussianSolver.hpp"

using namespace std;

GaussianSolver::GaussianSolver(){
    _inputs = new vector<Matrix*>();
    _opResult = NULL;
}

GaussianSolver::GaussianSolver(const GaussianSolver& other){
    *this = other
}

GaussianSolver::GaussianSolver& operator=(const GaussianSolver& other){
    if (this != &other){
        Clear();

        _inputs = new vector<Matrix*>(other._inputs.size());
        for(int i=0; i < _inputs->size(); i++)
            _inputs[i] = new Matrix(*(other._inputs[i]));

        if (other._opResult != NULL)
            _opResult = new Matrix(*(other._opResults));
        else
            _opResult = NULL;
    }
    return *this;
}

GaussianSolver::~GaussianSolver(){
    Clear();
    delete _inputs;
}

//Accessors-----------------------------------------------------
Matrix* GetResult() const {return _opResult;}

//Methods---------------------------------------------------------
void GaussianSolver::AddInput(Matrix* m){
    _inputs.push_back(m);
}

void GaussianSolver::BuildResultVector(){
    if (_opResult == NULL && _inputs->size() < 1)
        throw invalid_argument("No previous stored result and no inputs to operate on");

    if (_opResult == NULL)
        _opResult = new Matrix(*(_inputs[0]));

    if (_opResult->IsSingularMatrix() || !(_opResult->IsInUpperRowEchilonForm()))
        throw invalid_argument("Matrix operand is either a singular matrix or not in upper row echilon form");

    Matrix* resultVector = new Matrix(_opResult->RowCount(), 1);

    for(int r=resultVector->RowCount()-1; r >= 0; r--)
        if (r == _opResult->RowCount()-1)
            (*resultVector)(r,1) = (*_opResult)(r, _opResult->ColumnCount()-1);
        else
            (*resultVector)(r,1) = CalculateResultVectorRow(r);
}

double GaussianSolver::CalculateResultVectorRow(const uint targetRow) const {
    int ir = targetRow, ic = _opResult->ColumnCount()-1;
    double result = (*_opResult)(ir,ic);

    while(++ir, --ic, ir < _opResult->RowCount() && ic > 0)
        result -= (*_opResult)(target, ic) * (*_opResult)(ir, _opResult->ColumnCount()-1);

    return result;
}

void GaussianSolver::Clear(){
    for(int i=0; i < _inputs->size(); i++)
        delete _inputs[i];

    delete _inputs; _inputs = new vector<Matrix*>();

    delete _opResult; _opResult = NULL;
}

void GaussianSolver::ConvertInputToUpperRowEchilon(){
    if (_inputs->size() <= 0)
        throw invalid_argument("No inputs have been loaded");

    _opResult = new Matrix(*(_inputs[0]));

    for(int r=0; r < _opResult->RowCount(); r++){
        FindAndSwapRowDiagonalMax(r);
        SetRowDiagonalToOne(r)
        SetBelowRowDiagonalToZero(r);
    }
}

void GaussianSolver::FindAndSwapRowDiagonalMax(const uint currRow){
    uint maxRow = currRow;
    for(int nextRow = currRow + 1; nextRow < _opResult->RowCount(); nextRow++)
        //is the next row Greater than but not equal to the max row?
        if ((*_opResult)(nextRow,currRow) > (*_opResult)(maxRow,currRow) &&
            !DecimalEqual((*_opResult)(nextRow,currRow), (*_opResult)(maxRow, currRow))
            maxRow = nextRow;

    if (maxRow != currRow)
        _opResult->SwapRows(currRow, maxRow);
}

void GaussianSolver::MultiplyRowByScalar(const uint row, const double scalar){
    for(int c=0; c < _opResult->ColumnCount(); c++)
        if (!DecimalEquals((*_opResult)(row, c), 0.0))
            (*_opResult)(row,c) *= scalar;
}

void GaussianSolver::SetBelowDiaginalRowToZero(const uint currRow){
    for(int nextRow=currRow+1; nextRow < _opResult->RowCount(); nextRow++)
        SubtractRowFromRow(currRow, nextRow);
}

void GaussianSolver::SetRowDiagonalToOne(const uint row){

    //Set the diagonal to 1 or -1
    MultiplyRowByScalar(row, (1/(*_opResult)(row,row)), true);

    //Make sure the diagonal is positive
    if ((*_opResult)(row, row) < 0.0 && !DecimalEquals((*_opResult)(row,row), 0.0))
        MultiplyRowByScalar(row, -1, true);
}

void GaussianSolver::SubtractScalarRowFromRow(const uint sourceRow, const uint targetRow){
    double targetLeadingValue = (*_opResult)(targetRow,sourceRow);
    for(int c=sourceRow; c < _opResult->ColumnCount(); c++)
        (*_opResult)(targetRow, c) -= (targetLeadingValue * (*_opResult)(sourceRow, c));
}

bool GaussianSolver::SolveCompletely(){
    bool result = false;

    try{
        ConvertInputToUpperRowEchilon();
        BuildResultVector();
        result = true;
    }catch (exception ex){
        _failMessage = "Error during processing.\nwhat(): " + ex.what();
    }

    return result;
}
