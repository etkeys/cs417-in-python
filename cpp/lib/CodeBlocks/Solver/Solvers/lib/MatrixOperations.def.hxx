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
	static Matrix* Multiply(const Matrix*, const Matrix*);
	static Matrix* Subtract(const Matrix*, const Matrix*);

};

#endif
