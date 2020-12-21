#include <cmath>
#include <stdexcept>
#if DEBUG
    #include <sstream>
#endif
#ifndef ENABLETRACE_MATRIXOPERATIONS
    #ifdef TRACEMATRIX
        #define ENABLETRACE_MATRIXOPERATIONS true
    #else
        #define ENABLETRACE_MATRIXOPERATIONS false
    #endif
#endif
#ifndef METHODHEADER_MATRIXOPERATIONS
    #define METHODHEADER_MATRIXOPERATIONS Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIXOPERATIONS)
#endif

#include "Matrix.def.hxx"
#include "MatrixOperations.def.hxx"
#include "Utilities.hxx"

using namespace std;

Matrix* MatrixOperations::Add(const Matrix* m1, const Matrix* m2) {
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIXOPERATIONS);
    Matrix* result = Add(m1, m2, true);
    return result;
}

//When truely adding, scalar Adjust must be 1
//When subtracting, scalar Adjust must be -1
Matrix* MatrixOperations::Add(const Matrix* m1, const Matrix* m2, bool trueAddition){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIXOPERATIONS);
    if (!CanBeAdded(m1,m2))
        throw invalid_argument("Specified matrices are incompatible for addition.");

    Matrix* result = new Matrix(m1->RowCount(), m1->ColumnCount());
    int scalar = trueAddition ? 1 : -1;

    for(uint r=0; r < result->RowCount(); r++)
        for(uint c=0; c < result->ColumnCount(); c++)
            (*result)(r,c) = (*m1)(r,c) + ((*m2)(r,c) * scalar);

    return result;
}

bool MatrixOperations::CanBeAdded(const Matrix* m1, const Matrix* m2) {
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIXOPERATIONS);
    //When adding two matricies, their dimensions must be the same
    bool result = (m1->RowCount() == m2->RowCount() &&
                    m1->ColumnCount() == m2->ColumnCount());

    return result;
}

bool MatrixOperations::CanBeMultiplied(const Matrix* m1, const Matrix* m2) {
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIXOPERATIONS);
    //When multiplying two matrices mxn * hxj , the result will be
    //  a matrix mxj, but the condition (n == h) must be true

    bool result = (m1->ColumnCount() == m2->RowCount());
    return result;
}

Matrix* MatrixOperations::ConvertToReducedRowEchilonForm(const Matrix* src){
    METHODHEADER_MATRIXOPERATIONS;
    Matrix* result = NULL;
    if (!src->IsInUpperRowEchilonForm())
        result = MatrixOperations::ConvertToUpperRowEchilonForm(src);
    else
        result = new Matrix((*src));

    for(uint r = 0; r < result->RowCount(); r++)
        SetAboveDiagonalRowToZero(result, r);

    return result;
}

Matrix* MatrixOperations::ConvertToUpperRowEchilonForm(const Matrix* src){
    METHODHEADER_MATRIXOPERATIONS;

    if (src == NULL)
        throw invalid_argument("Input matrix cannot be null.");

    Matrix* result = new Matrix(*src);

    if (!result->IsInUpperRowEchilonForm())
        for(uint r=0; r < result->RowCount(); r++){
            FindAndSwapRowDiagonalMax(result, r);
            SetRowDiagonalToOne(result, r);
            SetBelowDiagonalRowToZero(result, r);
        }

    return result;
}

void MatrixOperations::FindAndSwapRowDiagonalMax(Matrix*& src, const uint currRow){
    METHODHEADER_MATRIXOPERATIONS;
    uint maxRow = currRow;
    for(uint nextRow = currRow + 1; nextRow < src->RowCount(); nextRow++)
        //is the next row Greater than but not equal to the max row?
        if ((*src)(nextRow,currRow) > (*src)(maxRow,currRow) &&
            !DoubleEquals((*src)(nextRow,currRow), (*src)(maxRow, currRow)))
            maxRow = nextRow;

    if (maxRow != currRow)
        src->SwapRows(currRow, maxRow);
}

Matrix* MatrixOperations::Multiply(const Matrix* m1, const Matrix* m2) {
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIXOPERATIONS);
    if (!CanBeMultiplied(m1,m2))
        throw invalid_argument("Specified matrices are incompatible for multiplication.");

    Matrix* result = new Matrix(m1->RowCount(), m2->ColumnCount());

    for(uint r=0; r < result->RowCount(); r++)
        for(uint c=0; c < m2->ColumnCount(); c++)
            (*result)(r,c) = MultiplyTargetIndexes(m1, m2, r, c);

    return result;
}

double MatrixOperations::MultiplyTargetIndexes(const Matrix* m1, const Matrix* m2, const uint sourceR, const uint sourceC){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIXOPERATIONS);
    double result = 0.0;
    uint iterR = 0, iterC = 0;
    //In matrix 1, we want to iterate over the columns, but always stay at the same row.
    //In matrix 2, we want to iterate over the rows, but always stay at the same column.

    do{
        result += (*m1)(sourceR, iterC) * (*m2)(iterR, sourceC);
    }while(++iterR, ++iterC, iterR < m1->ColumnCount() && iterC < m2->RowCount());

    return result;
}

Matrix* MatrixOperations::MultiplyByScalar(const Matrix* m, const double scalar){
    METHODHEADER_MATRIXOPERATIONS;
    Matrix* result = new Matrix((*m));

    for(uint r = 0; r < result->RowCount(); r++)
        result->MultiplyRowByScalar(r, scalar);

    return result;
}

void MatrixOperations::SetAboveDiagonalRowToZero(Matrix*& src, const uint currRow){
    METHODHEADER_MATRIXOPERATIONS;
    for(uint nextRow=currRow-1; nextRow < currRow; nextRow--)
        SubtractScalarRowFromRow(src, currRow, nextRow);
}

void MatrixOperations::SetBelowDiagonalRowToZero(Matrix*& src, const uint currRow){
    METHODHEADER_MATRIXOPERATIONS;
    for(uint nextRow=currRow+1; nextRow < src->RowCount(); nextRow++)
        SubtractScalarRowFromRow(src, currRow, nextRow);
}

void MatrixOperations::SetRowDiagonalToOne(Matrix*& result, const uint row){
    METHODHEADER_MATRIXOPERATIONS;

    //Set the diagonal to 1 or -1
    result->MultiplyRowByScalar(row, (1/(*result)(row,row)));

    //Make sure the diagonal is positive
    //C++11 use std::signbit()
    if (signbit((*result)(row,row)))
        result->MultiplyRowByScalar(row, -1.0);
}

Matrix* MatrixOperations::Subtract(const Matrix* m1, const Matrix* m2){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIXOPERATIONS);
    Matrix* result = Add(m1, m2, false);
    return result;
}

void MatrixOperations::SubtractScalarRowFromRow(Matrix*& src, const uint sourceRow, const uint targetRow){
    METHODHEADER_MATRIXOPERATIONS;
    double targetLeadingValue = (*src)(targetRow,sourceRow);
    for(uint c=sourceRow; c < src->ColumnCount(); c++)
        (*src)(targetRow, c) -= (targetLeadingValue * (*src)(sourceRow, c));

    //This is what happened:
    //src->MultiplyRowByScalar(sourceRow, targetLeadingValue) <- Makes the columns match
    //Subtract source row from target row <- Makes targer row zero
    //src->MultiplyRowByScalar(sourceRow, 1/targetLeadingValue) <- returns source to original
}
double MatrixOperations::TwoNormOfError(const Matrix* A, const Matrix* b, const Matrix* x){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIXOPERATIONS);
    double result = 0.0;
    //_inputs should have the original A, original B and
    //  the results from CalculateBackSolveVector. _inputs can have
    //  augmented A and results from CalculateBackSolveVector if A
    //  is Augmented
    if (A == NULL || b == NULL || x == NULL)
        throw invalid_argument("Either input MatrixA, MatrixB, or solution MatrixX is NULL");

    Matrix* ax = MatrixOperations::Multiply(A, x);
    Matrix* bax = MatrixOperations::Subtract(b, ax);

    for(uint r = 0; r < bax->RowCount(); r++)
        result += pow((*bax)(r,0), 2);

    result = sqrt(result);

    delete ax;
    delete bax;

    return result;
}
