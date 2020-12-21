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
	bool _isAugmented = false;

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
	int ColumnCount() const {Tracer _t_(__PRETTY_FUNCTION__); return _columns;}
	int RowCount() const {Tracer _t_(__PRETTY_FUNCTION__); return _rows;}

//Methods-----------------------------------------------------------------
    void Clear();
    static Matrix* CreateAFromAugmented(const Matrix*, const uint);
    static Matrix* CreateAugmentedMatrix(const Matrix*, const Matrix*);
    static Matrix* CreateBFromAugmented(const Matrix*, const uint);
	static Matrix* CreateFromInputStream(std::istream&, StreamType);
	static Matrix* CreateIdentity(unsigned int);
	static Matrix* CreateRandom(unsigned int, unsigned int);
	static Matrix* CreateRandomDiagonalDominate(unsigned int, unsigned int);
	Matrix* DeepCopy() const;
	bool IsAugmented() const;
	bool IsIdentity() const;
	bool IsInUpperRowEchilonForm() const;
	bool IsSingularMatrix() const;
	bool IsSquareMatrix() const;
	bool IsWithinBounds(unsigned int, unsigned int) const;
	void PrintPretty(std::ostream&) const;
	void SwapRows(unsigned int, unsigned int);

//Operators------------------------------------------------------
	double& operator() (unsigned int, unsigned int);
	double operator() (unsigned int, unsigned int) const;
};

std::ostream& operator<< (std::ostream&, const Matrix&);

#endif
