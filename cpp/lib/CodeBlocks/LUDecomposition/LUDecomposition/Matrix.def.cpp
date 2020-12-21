#include <cmath>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#ifdef DEBUG
	#include <sstream>
#endif
#ifndef ENABLETRACE_MATRIX
    #ifdef TRACEMATRIX
        #define ENABLETRACE_MATRIX true
    #else
        #define ENABLETRACE_MATRIX false
    #endif
#endif
#ifndef STDMETHODHEADER
    #define STDMETHODHEADER Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX)
#endif

#include "Matrix.def.hxx"
#include "MatrixOperations.def.hxx"
#include "Utilities.hxx"

using namespace std;

//Constructors----------------------------------------------------
Matrix::Matrix():
    _rows(0), _columns(0){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
    _data = NULL;
}
Matrix::Matrix(unsigned int upperBound):
    Matrix(upperBound, upperBound){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
}

Matrix::Matrix(uint rows, uint columns):
	_rows(rows), _columns(columns){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);

	if (rows==0 || columns==0)
		throw std::out_of_range("Matrix size cannot be 0.");

	_data = new double * [rows];

	for(uint r = 0; r < rows; r++)
		_data[r] = new double[columns];
}

Matrix::Matrix(const Matrix& other) {
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
	*this = other;
}

Matrix& Matrix::operator= (const Matrix& other){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
	if (this != &other){
		Clear();
        _isAugmented = other._isAugmented;
        _isInverted = other._isInverted;
		_rows = other._rows;
		_columns = other._columns;

		_data = new double* [_rows];

		for(uint r = 0; r < _rows; r++){
			_data[r] = new double[_columns];

			for(uint c = 0; c < _columns; c++)
				_data[r][c] = other(r,c);
        }
	}
	return *this;
}

Matrix::~Matrix(){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
	Clear();
}

//Accessors------------------------------------------------------
uint Matrix::ColumnCount() const {
    Tracer _t_(__PRETTY_FUNCTION__,false);
    return _columns;
}
uint Matrix::RowCount() const {
    Tracer _t_(__PRETTY_FUNCTION__,false);
    return _rows;
}

//Methods----------------------------------------------------------------
void Matrix::Clear(){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);

    if (_data != NULL && _rows != 0 ){
        for(uint r = 0; r < _rows; r++)
            delete[] _data[r];

        delete[] _data;
        _data = NULL;
        _rows = 0;
    }
}

double Matrix::ClosestValueToZero(double prev, double current, bool equalUsePositive) const {
    STDMETHODHEADER;
    double prevAbs = fabs(prev);
    double currentAbs = fabs(current);
    double result = prev;

    if (DoubleEquals(prevAbs,currentAbs)){//are the values equadistant from zero?
        if (!DoubleEquals(prev,current)) //are the values truely the same?
            result = equalUsePositive ? max(prev, current) : min(prev, current);

    }else if (DoubleEquals(max(prevAbs, currentAbs),prevAbs)) //new index is closer to zero
        result = current;

    return result;
}

double Matrix::ClosestValueToZeroInColumn(uint index) const {
    return ClosestValueToZeroInColumn(index, true);
}

double Matrix::ClosestValueToZeroInColumn(uint index, bool equadistantPreferPositive) const {
    STDMETHODHEADER;
    IsWithinBounds(0, index);
    double result = (*this)(0,index);

    for(uint r = 0; r < this->RowCount(); r++)
        result = ClosestValueToZero(result, (*this)(r,index), equadistantPreferPositive);

    return result;
}

double Matrix::ClosestValueToZeroInRow(uint index) const{
    return ClosestValueToZeroInRow(index, true);
}

double Matrix::ClosestValueToZeroInRow(uint index, bool equadistantPreferPositive) const {
    STDMETHODHEADER;
    IsWithinBounds(index, 0);
    double result = (*this)(index, 0);

    for(uint c = 0; c < this->ColumnCount(); c++)
        result = ClosestValueToZero(result, (*this)(index, c), equadistantPreferPositive);

    return result;
}

Matrix* Matrix::CreateAFromAugmented(const Matrix* src, const uint firstBColumn){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
    Matrix* result = Matrix::ExtractMatrixFromAugmented(src, 0, firstBColumn);
    return result;
}

Matrix* Matrix::CreateAugmentedMatrix(const Matrix* m1, const Matrix* m2){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
    if (m1->RowCount() != m2->RowCount())
        throw invalid_argument("M1 & M2 row counts do not match.");

    Matrix* result = new Matrix(m1->RowCount(), m1->ColumnCount() + m2->ColumnCount());
    result->_isAugmented = true;
    for(uint r=0; r < m1->RowCount(); r++){
        for(uint c=0; c < m1->ColumnCount(); c++)
            (*result)(r,c) = (*m1)(r,c);

        for(uint c2=0; c2 < m2->ColumnCount(); c2++)
            (*result)(r, c2+m1->ColumnCount()) = (*m2)(r,c2);
    }

    return result;
}

Matrix* Matrix::CreateBFromAugmented(const Matrix* src, uint firstBColumn){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
    Matrix* result = Matrix::ExtractMatrixFromAugmented(src, firstBColumn, src->ColumnCount());
    return result;
}

Matrix* Matrix::CreateDiagonalTermMatrix(const Matrix* src){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
    return Matrix::CreateDiagonalNonDiagonalTermMatrix(src, true);
}

Matrix* Matrix::CreateDiagonalNonDiagonalTermMatrix(const Matrix* src, bool getDiagonal){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);

    if (!src->IsSquareMatrix())
        throw invalid_argument("Source matrix must be a square matrix.");

    Matrix* result = new Matrix(src->RowCount(), src->ColumnCount());

    for(uint r = 0; r < src->RowCount(); r++)
        for(uint c = 0; c < src->ColumnCount(); c++)
            if (r == c)
                (*result)(r,c) = getDiagonal ? (*src)(r,c) : 0.0;
            else
                (*result)(r,c) = getDiagonal ? 0.0 : (*src)(r,c);

    return result;
}

Matrix* Matrix::CreateFromInputStream(istream& in, Matrix::StreamType type){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
    Matrix* result = nullptr;
    int rows=0, columns=0;

    if (type == Matrix::StreamType::BasicText || type == Matrix::StreamType::StdIn){
        if (in >> rows >> columns){

#ifdef DEBUG
            stringstream ss;
            ss << "\t\trows=" << rows << ";columns=" << columns;
            PrintDebug(cerr, ss.str());
#endif

            if (rows < 0 || columns < 0)
                throw overflow_error("Row or column limits cannot be negative.");

            result = new Matrix((uint)rows, (uint)columns);

            for(int r=0; r<rows; r++)
                for(int c=0; c<columns; c++)
                    in >> (*result)(r,c);

        }else
            throw runtime_error("Could not read from istream&.");
    }
    return result;
}

Matrix* Matrix::CreateIdentity(uint upperBound){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
    Matrix* result = new Matrix(upperBound);

    for(uint r=0; r<upperBound; r++)
        for(uint c=0; c<upperBound; c++)
            if (r==c)
                (*result)(r,c) = 1.0;
            else
                (*result)(r,c) = 0.0;

    return result;
}

Matrix* Matrix::CreateLComponentFromMatrix(const Matrix* src){
    STDMETHODHEADER;
    if (!src->IsSquareMatrix())
        throw invalid_argument("Source matrix is not square.");

    Matrix* result = new Matrix((*src));

    for(uint r = 0; r < result->RowCount(); r++)
        for(uint c = r+1; c < result->ColumnCount(); c++)
            (*result)(r,c) = 0.0;

    return result;
}

Matrix* Matrix::CreateNonDiagonalTermMatrix(const Matrix* src){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
    return Matrix::CreateDiagonalNonDiagonalTermMatrix(src, false);
}

Matrix* Matrix::CreateRandom(uint rows, uint columns){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
    Matrix* result = new Matrix(rows, columns);

    //http://www.cplusplus.com/reference/random/linear_congruential_engine/linear_congruential_engine/
    default_random_engine dre(chrono::system_clock::now().time_since_epoch().count());
    uniform_real_distribution<double> urd(-20,20);

    for(uint r=0; r<rows; r++)
        for(uint c=0; c<columns; c++)
            (*result)(r,c) = urd(dre);

    return result;
}

Matrix* Matrix::CreateRandomDiagonalDominate(uint rows, uint columns){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
    const double arbitraryScalar = 10.0;
    Matrix* result = Matrix::CreateRandom(rows, columns);

    if (!result->IsSquareMatrix())
        throw invalid_argument("Requested Matrix is not square.");

    for(uint r=0; r<rows; r++){
        double rowSum = 0.0;

        for(uint c=0; c<columns; c++)
            if(c != r)
                rowSum += abs((*result)(r,c));

        (*result)(r,r) = rowSum * arbitraryScalar;
    }
    return result;
}

Matrix* Matrix::CreateUComponentFromMatrix(const Matrix* src, bool strict){
    STDMETHODHEADER;
    if (!src->IsSquareMatrix())
        throw invalid_argument("Source matrix is not square.");

    Matrix* result = new Matrix((*src));

    for (uint r = 0; r < result->RowCount(); r++)
        for (uint c = 0; c <= r; c++)
            if (c != r || strict)
                (*result)(r,c) = 0.0;
            else
                (*result)(r,c) = 1.0;

    return result;
}

Matrix* Matrix::DeepCopy() const{
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
	Matrix* result = new Matrix(*this);
	return result;
}

void Matrix::DiagonalOnlyInvert(){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);

    for (uint index = 0; index < this->RowCount(); index++)
        (*this)(index, index) = (1 / (*this)(index, index));
}

void Matrix::EntireMatrixInvert(){
    STDMETHODHEADER;
    Matrix* aug = Matrix::CreateAugmentedMatrix(this, Matrix::CreateIdentity(this->RowCount()));

    aug->ToReducedRowEchilonForm(); // A|I --> I|(A^-1)

    Matrix* temp = Matrix::CreateBFromAugmented(aug, aug->RowCount()); // returns A^-1

    (*this) = (*temp);

    delete aug;
    delete temp;
}

Matrix* Matrix::ExtractMatrixFromAugmented(const Matrix* src, const uint lbound, const uint ubound){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
    if (!src->IsAugmented())
        throw invalid_argument("Source matrix is not augmented.");

    Matrix* result = new Matrix(src->RowCount(), ubound-lbound);

    for(uint r = 0; r < result->RowCount(); r++)
        for(uint c = 0; c < result->ColumnCount(); c++)
            (*result)(r,c) = (*src)(r,lbound+c);

    return result;
}

void Matrix::Invert(){
    STDMETHODHEADER;

    if (!this->IsSquareMatrix() || this->IsSingularMatrix())
        throw runtime_error("Only square, non-singular matrices have an inverse.");

    if (this->IsDiagonalTermMatrix())
        DiagonalOnlyInvert();
    else
        EntireMatrixInvert();

    _isInverted = !_isInverted;
}

bool Matrix::IsAugmented() const{
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
    return _isAugmented;
}

bool Matrix::IsDiagonalTermMatrix() const {
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
    bool hasCorrectValues = this->IsSquareMatrix();

    for (uint r = 0; hasCorrectValues && r < this->RowCount(); r++)
        for (uint c = 0; hasCorrectValues && c < this->ColumnCount(); c++)
            if (r != c && !DoubleEquals((*this)(r,c), 0.0))
                hasCorrectValues = false;

    return hasCorrectValues;
}

bool Matrix::IsIdentity() const{
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
	bool result = false;

	if (_rows != _columns)
		result = false;
	else{
		//The diagonal from top->down, left->right must all be 1
		//all other must be 0
        PrintDebug(cerr, "\t\tSearching matrix");
		bool hasCorrectValues = true;
		for(uint r=0; hasCorrectValues && r < _rows; r++)
			for (uint c=0; hasCorrectValues && c < _columns; c++)
				if (r == c && !DoubleEquals(_data[r][c],1.0))
					hasCorrectValues = false;
				else if (r != c && !DoubleEquals(_data[r][c],0.0))
					hasCorrectValues = false;

		result = hasCorrectValues;
	}

	return result;
}

bool Matrix::IsInCroutLForm() const{
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
    bool hasCorrectValues = true;

    //Need to make sure that all numbers to the right of diagonal
    //  are zero, everything doesn't really matter.
    for (uint r = 0; hasCorrectValues && r < this->RowCount(); r++)
        for(uint c = r + 1; hasCorrectValues && c < this->ColumnCount(); c++)
            if (!DoubleEquals((*this)(r,c), 0.0))
                hasCorrectValues = false;

    return hasCorrectValues;
}

bool Matrix::IsInverted() const{
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
    return _isInverted;
}

bool Matrix::IsInUpperRowEchilonForm() const{
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
    bool result = false;

    if (_rows <= _columns){
        bool hasCorrectValue = true;

        for(uint r=0; hasCorrectValue && r < _rows; r++)
            for(uint c = 0; hasCorrectValue && c <= r; c++)
                if (r==c && !DoubleEquals(_data[r][c], 1.0))
                    hasCorrectValue = false;
                else if (r!=c && !DoubleEquals(_data[r][c], 0.0))
                    hasCorrectValue = false;

        result = hasCorrectValue;
    }
    return result;
}

bool Matrix::IsSquareMatrix() const{
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
    bool result = false;

    if (_rows > 0 && _columns > 0)
        if (_rows==_columns)
            result = true;

    return result;
}

bool Matrix::IsWithinBounds(unsigned int row, unsigned int column) const{
    //Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
	if (row >= _rows || column >= _columns)
        return false;
    else
        return true;
}

bool Matrix::IsSingularMatrix() const {
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
    bool result = false;

    if (_rows == _columns || _columns == _rows + 1)
        for(uint r=0; !result && r<_rows; r++)
            for(uint c=r; c==r; c++)
                if (DoubleEquals(_data[r][c],0.0))
                    result = true;

    return result;
}

double Matrix::MaxOrMinValueInColumn(uint index, bool getMax) const{
    STDMETHODHEADER;
    IsWithinBounds(0,index);
    double result = (*this)(0,index);

    for(uint r = 0; r < this->RowCount(); r++)
        if (getMax)
            result = max(result, (*this)(r,index));
        else
            result = min(result, (*this)(r,index));

    return result;
}

double Matrix::MaxOrMinValueInRow(uint index, bool getMax)const{
    STDMETHODHEADER;
    IsWithinBounds(index,0);
    double result = (*this)(index, 0);

    for(uint c = 0; c < this->ColumnCount(); c++)
        if (getMax)
            result = max(result, (*this)(index,c));
        else
            result = min(result, (*this)(index,c));

    return result;
}

void Matrix::MultiplyRowByScalar(const uint row, const double scalar){
    STDMETHODHEADER;
    for(uint c=0; c < this->ColumnCount(); c++)
        if (!DoubleEquals((*this)(row, c), 0.0))
            (*this)(row,c) *= scalar;
}

void Matrix::PrintPretty(ostream& out) const{
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
    std::streamsize ssize = out.precision();
    out << '[' << endl;

    for(uint r=0; r<_rows; r++){
        out << '[';

        for(uint c=0; c<_columns; c++)
            out << right << setprecision(8) << fixed << setw(15) << _data[r][c];
            //out << left << setw(12) << (*this)(r,c);

        out << ']' << endl;
    }

    out << ']' << std::endl;
    out << setprecision(ssize);
}

void Matrix::SwapRows(unsigned int row1, unsigned int row2){
    STDMETHODHEADER;
    if (IsWithinBounds(row1, 0) && IsWithinBounds(row2, 0)){

        double * temp = _data[row1];
        _data[row1] = _data[row2];
        _data[row2] = temp;

        temp = NULL;
    }else
        throw std::out_of_range("Either row or column outside Matrix bounds.");
}

void Matrix::ToReducedRowEchilonForm(){
    STDMETHODHEADER;
    (*this) = (*MatrixOperations::ConvertToReducedRowEchilonForm(this));
}

void Matrix::ToUpperRowEchilonForm(){
    STDMETHODHEADER;
    (*this) = (*MatrixOperations::ConvertToUpperRowEchilonForm(this));
}


//Operators---------------------------------------------------------
double& Matrix::operator() (uint row, uint column){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
	if (IsWithinBounds(row, column))
        return _data[row][column];
    else
        throw std::out_of_range("Either row or column outside Matrix bounds.");
}

double Matrix::operator() (uint row, uint column) const{
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
	if (IsWithinBounds(row, column))
        return _data[row][column];
    else
        throw std::out_of_range("Either row or column outside Matrix bounds.");
}

bool Matrix::operator==(const Matrix &rhs) const{
    STDMETHODHEADER;
    bool result = false;

   if (this->RowCount() == rhs.RowCount() &&
            this->ColumnCount() == rhs.ColumnCount()){

        bool hasMatchingValues = true;
        for(uint r = 0; hasMatchingValues && r < this->RowCount(); r++)
            for(uint c = 0; hasMatchingValues && c < this->ColumnCount(); c++){
#ifdef DEBUG
    stringstream ss;
    ss << "\t\tEquality compare this{" << (*this)(r,c)
            << "} == that{" << rhs(r,c) << "}";
    PrintDebug(cerr, ss.str());
#endif
                hasMatchingValues = (DoubleEquals((*this)(r,c), rhs(r,c)));
            }

            result = hasMatchingValues;
    }

    return result;
}

bool Matrix::operator!=(const Matrix &rhs) const{
    return !(*this == rhs);
}

ostream& operator<< (ostream& out, const Matrix& m){
    Tracer _t_(__PRETTY_FUNCTION__,ENABLETRACE_MATRIX);
    out << m.RowCount() << ' ' << m.ColumnCount();

    for(uint r=0; r < m.RowCount(); r++)
        for(uint c=0; c < m.ColumnCount(); c++)
            out << ' ' << m(r,c);

    return out;
}

