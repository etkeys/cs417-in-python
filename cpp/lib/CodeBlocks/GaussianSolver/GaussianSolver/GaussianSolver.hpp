#ifndef GAUSSIANSOLVER_H
#define GAUSSIANSOLVER_H

#include <string>
#include <vector>

#include "../../Matrix/Matrix/Utilities.hpp"

//Foward declaration
class Matrix;

class GaussianSolver {
private:
    std::vector<Matrix*>* _inputs;
    Matrix* _opResult;

    std::string _failMessage;

    double CalculateResultVectorRow(const uint);
    void FindAndSwapRowDiagonalMax(const uint);
	void MultiplyRowByScalar(const uint, const double);
	void SetBelowDiagonalRowToZero(const uint);
	void SetRowDiagonalToOne(const uint);
	void SubtractScalarRowFromRow(const uint, const uint);

public:
    GaussianSolver();
    GaussianSolver(const GaussianSolver&);
    ~GaussianSolver();
    GaussianSolver& operator=(const GaussianSolver&);

    std::string GetFailMessage() const {return _failMessage;}
    Matrix* GetResult() const;

    void AddInput(Matrix*);
    void BuildResultVector();
    void ConvertInputToUpperRowEchilon();
    void Clear();
    bool SolveCompletely();
};
#endif
