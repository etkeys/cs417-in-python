#include <stdexcept>
#if DEBUG
    #include <sstream>
#endif

#include "Matrix.def.hxx"
#include "MatrixOperations.def.hxx"
#include "Utilities.hxx"

using namespace std;

Matrix* MatrixOperations::Add(const Matrix* m1, const Matrix* m2) {
    Tracer _t_(__PRETTY_FUNCTION__);
    Matrix* result = Add(m1, m2, true);
    return result;
}

//When truely adding, scalar Adjust must be 1
//When subtracting, scalar Adjust must be -1
Matrix* MatrixOperations::Add(const Matrix* m1, const Matrix* m2, bool trueAddition){
    Tracer _t_(__PRETTY_FUNCTION__);
    if (!CanBeAdded(m1,m2))
        throw invalid_argument("Specified matrices are incompatible for addition.");

    Matrix* result = new Matrix(m1->RowCount(), m1->ColumnCount());
    int scalar = trueAddition ? 1 : -1;

    for(int r=0; r < result->RowCount(); r++)
        for(int c=0; c < result->ColumnCount(); c++)
            (*result)(r,c) = (*m1)(r,c) + ((*m2)(r,c) * scalar);

    return result;
}

bool MatrixOperations::CanBeAdded(const Matrix* m1, const Matrix* m2) {
    Tracer _t_(__PRETTY_FUNCTION__);
    //When adding two matricies, their dimensions must be the same
    bool result = (m1->RowCount() == m2->RowCount() &&
                    m1->ColumnCount() == m2->ColumnCount());

    return result;
}

bool MatrixOperations::CanBeMultiplied(const Matrix* m1, const Matrix* m2) {
    Tracer _t_(__PRETTY_FUNCTION__);
    //When multiplying two matrices mxn * hxj , the result will be
    //  a matrix mxj, but the condition (n == h) must be true

    bool result = (m1->ColumnCount() == m2->RowCount());
    return result;
}

Matrix* MatrixOperations::Multiply(const Matrix* m1, const Matrix* m2) {
    Tracer _t_(__PRETTY_FUNCTION__);
    if (!CanBeMultiplied(m1,m2))
        throw invalid_argument("Specified matrices are incompatible for multiplication.");

    Matrix* result = new Matrix(m1->RowCount(), m2->ColumnCount());

    for(uint r=0; r < result->RowCount(); r++)
        for(uint c=0; c < m2->ColumnCount(); c++)
            (*result)(r,c) = MultiplyTargetIndexes(m1, m2, r, c);

    return result;
}

double MatrixOperations::MultiplyTargetIndexes(const Matrix* m1, const Matrix* m2, const uint sourceR, const uint sourceC){
    Tracer _t_(__PRETTY_FUNCTION__);
    double result = 0.0;
    uint iterR = 0, iterC = 0;
    //In matrix 1, we want to iterate over the columns, but always stay at the same row.
    //In matrix 2, we want to iterate over the rows, but always stay at the same column.

    do{
#if DEBUG
        stringstream ss;
        ss << "\t\tSourceR=" << sourceR << ";IterC=" << iterC
            << ";SourceC=" << sourceC << ";IterR=" << iterR;
        PrintDebug(cerr, ss.str());
#endif
        result += (*m1)(sourceR, iterC) * (*m2)(iterR, sourceC);
    }while(++iterR, ++iterC, iterR < m1->ColumnCount() && iterC < m2->RowCount());

    return result;
}

Matrix* MatrixOperations::Subtract(const Matrix* m1, const Matrix* m2){
    Tracer _t_(__PRETTY_FUNCTION__);
    Matrix* result = Add(m1, m2, false);
    return result;
}
