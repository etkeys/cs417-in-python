#ifndef MATRIXOPERATIONS_DEF_H
#define MATRIXOPERATIONS_DEF_H

#include "Utilities.hxx"

class Matrix;

class MatrixOperations{
private:

    static Matrix* Add(const Matrix*, const Matrix*, bool);
    static double MultiplyTargetIndexes(const Matrix*, const Matrix*, const uint, const uint);
public:
	//TODO need the following functionality
	//Add
	//	bool CanBeAdded(m1, m2)
	//Multiply
	//	bool CanBeMultiplied(m1, m2)
	//Cross product
	//Dot product

	static Matrix* Add(const Matrix*, const Matrix*);
	static bool CanBeAdded(const Matrix*, const Matrix*);
	static bool CanBeMultiplied(const Matrix*, const Matrix*);
	static Matrix* ConvertToReducedRowEchilonForm(const Matrix*);
	static Matrix* ConvertToUpperRowEchilonForm(const Matrix*);
	static void FindAndSwapRowDiagonalMax(Matrix*&, const uint);
	static Matrix* Multiply(const Matrix*, const Matrix*);
	static Matrix* MultiplyByScalar(const Matrix*, const double);
	static void SetAboveDiagonalRowToZero(Matrix*&, const uint);
	static void SetBelowDiagonalRowToZero(Matrix*&, const uint);
	static void SetRowDiagonalToOne(Matrix*&, const uint);
	static Matrix* Subtract(const Matrix*, const Matrix*);
	static void SubtractScalarRowFromRow(Matrix*&, const uint, const uint);

	//For use with equation form Ax=b
	static double TwoNormOfError(const Matrix*, const Matrix*, const Matrix*);
};

#endif
