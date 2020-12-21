#include <cmath>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#ifdef DEBUG
	#include <sstream>
#endif

#include "Matrix.def.hxx"
#include "Utilities.hxx"

using namespace std;

//Constructors----------------------------------------------------
Matrix::Matrix():
    _rows(0), _columns(0){
    Tracer _t_(__PRETTY_FUNCTION__);
    _data = NULL;
}
Matrix::Matrix(unsigned int upperBound):
    Matrix(upperBound, upperBound){
    Tracer _t_(__PRETTY_FUNCTION__);
}

Matrix::Matrix(unsigned int rows, unsigned int columns):
	_rows(rows), _columns(columns){
    Tracer _t_(__PRETTY_FUNCTION__);

	if (rows==0 || columns==0)
		throw std::out_of_range("Matrix size cannot be 0.");

	_data = new double * [rows];

	for(int r = 0; r < rows; r++)
		_data[r] = new double[columns];
}

Matrix::Matrix(const Matrix& other) {
    Tracer _t_(__PRETTY_FUNCTION__);
	*this = other;
}

Matrix& Matrix::operator= (const Matrix& other){
    Tracer _t_(__PRETTY_FUNCTION__);
	if (this != &other){
		Clear();

		_rows = other._rows;
		_columns = other._columns;

		_data = new double* [_rows];

		for(int r = 0; r < _rows; r++){
			_data[r] = new double[_columns];

			for(int c = 0; c < _columns; c++)
				_data[r][c] = other(r,c);
        }
	}
	return *this;
}

Matrix::~Matrix(){
    Tracer _t_(__PRETTY_FUNCTION__);
	Clear();
}

//Methods----------------------------------------------------------------
void Matrix::Clear(){
    Tracer _t_(__PRETTY_FUNCTION__);

    if (_rows != 0 && _data != NULL){
        for(int r = 0; r < _rows; r++)
            delete[] _data[r];

        delete[] _data; _data = NULL;
	}
}

Matrix* Matrix::CreateAFromAugmented(const Matrix* src, const uint firstBColumn){
    Tracer _t_(__PRETTY_FUNCTION__);
    Matrix* result = Matrix::ExtractMatrixFromAugmented(src, 0, firstBColumn);
    return result;
}

Matrix* Matrix::CreateAugmentedMatrix(const Matrix* m1, const Matrix* m2){
    Tracer _t_(__PRETTY_FUNCTION__);
    if (m1->RowCount() != m2->RowCount())
        throw invalid_argument("M1 & M2 row counts do not match.");

    Matrix* result = new Matrix(m1->RowCount(), m1->ColumnCount() + m2->ColumnCount());
    result->_isAugmented = true;
    for(int r=0; r < m1->RowCount(); r++){
        for(int c=0; c < m1->ColumnCount(); c++)
            (*result)(r,c) = (*m1)(r,c);

        for(int c2=0; c2 < m2->ColumnCount(); c2++)
            (*result)(r, c2+m1->ColumnCount()) = (*m2)(r,c2);
    }

    return result;
}

Matrix* Matrix::CreateBFromAugmented(const Matrix* src, uint firstBColumn){
    Tracer _t_(__PRETTY_FUNCTION__);
    Matrix* result = Matrix::ExtractMatrixFromAugmented(src, firstBColumn, src->ColumnCount());
    return result;
}

Matrix* Matrix::CreateFromInputStream(istream& in, Matrix::StreamType type){
    Tracer _t_(__PRETTY_FUNCTION__);
    Matrix* result;
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

            result = new Matrix((unsigned int)rows, (unsigned int)columns);

            for(int r=0; r<rows; r++)
                for(int c=0; c<columns; c++)
                    in >> (*result)(r,c);

        }else
            throw runtime_error("Could not read from istream&.");
    }
    return result;
}

Matrix* Matrix::CreateIdentity(unsigned int upperBound){
    Tracer _t_(__PRETTY_FUNCTION__);
    Matrix* result = new Matrix(upperBound);

    for(int r=0; r<upperBound; r++)
        for(int c=0; c<upperBound; c++)
            if (r==c)
                (*result)(r,c) = 1.0;
            else
                (*result)(r,c) = 0.0;

    PrintDebug(cerr, "\t\tCreateIdentity internal results:");
    result->PrintPretty(cerr);

    return result;
}

Matrix* Matrix::CreateRandom(unsigned int rows, unsigned int columns){
    Tracer _t_(__PRETTY_FUNCTION__);
    Matrix* result = new Matrix(rows, columns);

    //http://www.cplusplus.com/reference/random/linear_congruential_engine/linear_congruential_engine/
    default_random_engine dre(chrono::system_clock::now().time_since_epoch().count());
    uniform_real_distribution<double> urd(-20,20);

    for(int r=0; r<rows; r++)
        for(int c=0; c<columns; c++)
            (*result)(r,c) = urd(dre);

    return result;
}

Matrix* Matrix::CreateRandomDiagonalDominate(unsigned int rows, unsigned int columns){
    Tracer _t_(__PRETTY_FUNCTION__);
    const double arbitraryScalar = 10.0;
    Matrix* result = Matrix::CreateRandom(rows, columns);

    if (!result->IsSquareMatrix())
        throw invalid_argument("Requested Matrix is not square.");

    for(int r=0; r<rows; r++){
        double rowSum = 0.0;

        for(int c=0; c<columns; c++)
            if(c != r)
                rowSum += abs((*result)(r,c));

        (*result)(r,r) = rowSum * arbitraryScalar;
    }
    return result;
}

Matrix* Matrix::DeepCopy() const{
    Tracer _t_(__PRETTY_FUNCTION__);
	Matrix* result = new Matrix(*this);
	return result;
}

Matrix* Matrix::ExtractMatrixFromAugmented(const Matrix* src, const uint lbound, const uint ubound){
    Tracer _t_(__PRETTY_FUNCTION__);
    if (!src->IsAugmented())
        throw invalid_argument("Source matrix is not augmented.");

    Matrix* result = new Matrix(src->RowCount(), ubound-lbound);

    for(uint r = 0; r < result->RowCount(); r++)
        for(uint c = 0; c < result->ColumnCount(); c++)
            (*result)(r,c) = (*src)(r,lbound+c);

    return result;
}

bool Matrix::IsAugmented() const{
    Tracer _t_(__PRETTY_FUNCTION__);
    return _isAugmented;
}

bool Matrix::IsIdentity() const{
    Tracer _t_(__PRETTY_FUNCTION__);
	bool result = false;

	if (_rows != _columns)
		result = false;
	else{
		//The diagonal from top->down, left->right must all be 1
		//all other must be 0
        PrintDebug(cerr, "\t\tSearching matrix");
		bool hasCorrectValues = true;
		for(int r=0; hasCorrectValues && r < _rows; r++)
			for (int c=0; hasCorrectValues && c < _columns; c++)
				if (r == c && !DoubleEquals(_data[r][c],1.0))
					hasCorrectValues = false;
				else if (r != c && !DoubleEquals(_data[r][c],0.0))
					hasCorrectValues = false;

		result = hasCorrectValues;
	}

	return result;
}

bool Matrix::IsInUpperRowEchilonForm() const{
    Tracer _t_(__PRETTY_FUNCTION__);
    bool result = false;

    if (_rows <= _columns){
        bool hasCorrectValue = true;

        for(int r=0; hasCorrectValue && r < _rows; r++)
            for(int c = 0; hasCorrectValue && c <= r; c++)
                if (r==c && !DoubleEquals(_data[r][c], 1.0))
                    hasCorrectValue = false;
                else if (r!=c && !DoubleEquals(_data[r][c], 0.0))
                    hasCorrectValue = false;

        result = hasCorrectValue;
    }
    return result;
}

bool Matrix::IsSquareMatrix() const{
    Tracer _t_(__PRETTY_FUNCTION__);
    bool result = false;

    if (_rows > 0 && _columns > 0)
        if (_rows==_columns)
            result = true;

    return result;
}

bool Matrix::IsWithinBounds(unsigned int row, unsigned int column) const{
    Tracer _t_(__PRETTY_FUNCTION__);
	if (row >= _rows || column >= _columns)
        return false;
    else
        return true;
}

bool Matrix::IsSingularMatrix() const {
    Tracer _t_(__PRETTY_FUNCTION__);
    bool result = false;

    if (_rows == _columns || _columns == _rows + 1)
        for(int r=0; !result && r<_rows; r++)
            for(int c=r; c==r; c++)
                if (DoubleEquals(_data[r][c],0.0))
                    result = true;

    return result;
}

void Matrix::PrintPretty(ostream& out) const{
    Tracer _t_(__PRETTY_FUNCTION__);
    std::streamsize ssize = out.precision();
    out << '[' << endl;

    for(int r=0; r<_rows; r++){
        out << '[';

        for(int c=0; c<_columns; c++)
            out << left << setprecision(2) << fixed << setw(10) << _data[r][c];

        out << ']' << endl;
    }

    out << ']' << std::endl;
    out << setprecision(ssize);
}

void Matrix::SwapRows(unsigned int row1, unsigned int row2){
    Tracer _t_(__PRETTY_FUNCTION__);
    if (IsWithinBounds(row1, 0) && IsWithinBounds(row2, 0)){

        double * temp = _data[row1];
        _data[row1] = _data[row2];
        _data[row2] = temp;

        temp = NULL;
    }else
        throw std::out_of_range("Either row or column outside Matrix bounds.");
}

//Operators---------------------------------------------------------
double& Matrix::operator() (unsigned int row, unsigned int column){
    Tracer _t_(__PRETTY_FUNCTION__);
	if (IsWithinBounds(row, column))
        return _data[row][column];
    else
        throw std::out_of_range("Either row or column outside Matrix bounds.");
}

double Matrix::operator() (unsigned int row, unsigned int column) const{
    Tracer _t_(__PRETTY_FUNCTION__);
	if (IsWithinBounds(row, column))
        return _data[row][column];
    else
        throw std::out_of_range("Either row or column outside Matrix bounds.");
}

ostream& operator<< (ostream& out, const Matrix& m){
    Tracer _t_(__PRETTY_FUNCTION__);
    out << m.RowCount() << ' ' << m.ColumnCount();

    for(int r=0; r < m.RowCount(); r++)
        for(int c=0; c < m.ColumnCount(); c++)
            out << ' ' << m(r,c);

    return out;
}

