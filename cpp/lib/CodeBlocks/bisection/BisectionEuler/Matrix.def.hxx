#ifndef MATRIX_DEF_H
#define MATRIX_DEF_H

#include <iostream>
#include "Utilities.hxx"

//Some code constructs taken from:
//https://isocpp.org/wiki/faq/operator-overloading#matrix-subscript-op

class Matrix{
private:
	unsigned int _rows, _columns;
	double** _data = NULL;
	bool _isAugmented = false, _isInverted = false;

    double ClosestValueToZero(double,double,bool)const;
    static Matrix* CreateDiagonalNonDiagonalTermMatrix(const Matrix*, bool);
    void DiagonalOnlyInvert();
    void EntireMatrixInvert();
	static Matrix* ExtractMatrixFromAugmented(const Matrix*, const uint, const uint);

public:
    enum StreamType {BasicText, StdIn};

//Constructors/Destructors----------------------------------------
	Matrix();
	Matrix(unsigned int);
	Matrix(unsigned int, unsigned int);

	//Big 3
	Matrix(const Matrix&); 	//Copy constructor
	Matrix& operator= (const Matrix&);	//Assignment operator
	~Matrix();	//Destructor

//Accessors------------------------------------------------------
	int ColumnCount() const;
	int RowCount() const;

//Methods-----------------------------------------------------------------
    void Clear();
    double ClosestValueToZeroInColumn(uint) const;
    double ClosestValueToZeroInColumn(uint,bool) const;
    double ClosestValueToZeroInRow(uint) const;
    double ClosestValueToZeroInRow(uint,bool) const;
    static Matrix* CreateAFromAugmented(const Matrix*, const uint);
    static Matrix* CreateAugmentedMatrix(const Matrix*, const Matrix*);
    static Matrix* CreateBFromAugmented(const Matrix*, const uint);
    static Matrix* CreateDiagonalTermMatrix(const Matrix*);
	static Matrix* CreateFromInputStream(std::istream&, StreamType);
	static Matrix* CreateIdentity(unsigned int);
	static Matrix* CreateLComponentFromMatrix(const Matrix*);
	static Matrix* CreateNonDiagonalTermMatrix(const Matrix*);
	static Matrix* CreateRandom(unsigned int, unsigned int);
	static Matrix* CreateRandomDiagonalDominate(unsigned int, unsigned int);
	static Matrix* CreateUComponentFromMatrix(const Matrix*, bool);
	Matrix* DeepCopy() const;
	void Invert();
	bool IsAugmented() const;
	bool IsDiagonalTermMatrix() const;
	bool IsIdentity() const;
	bool IsInCroutLForm() const;
	bool IsInverted() const;
	bool IsInUpperRowEchilonForm() const;
	bool IsSingularMatrix() const;
	bool IsSquareMatrix() const;
	bool IsWithinBounds(unsigned int, unsigned int) const;
	double MaxOrMinValueInColumn(uint,bool) const;
	double MaxOrMinValueInRow(uint,bool) const;
	void MultiplyRowByScalar(const uint, const double);
	void PrintPretty(std::ostream&) const;
	void SwapRows(unsigned int, unsigned int);
	void ToReducedRowEchilonForm();
	void ToUpperRowEchilonForm();

//Operators------------------------------------------------------
	double& operator() (uint, uint);
	double operator() (uint, uint) const;

	bool operator==(const Matrix&)const;
	bool operator!=(const Matrix&)const;
};

std::ostream& operator<< (std::ostream&, const Matrix&);

#endif
