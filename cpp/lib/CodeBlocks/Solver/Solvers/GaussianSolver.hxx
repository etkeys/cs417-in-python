#ifndef GAUSSIANSOLVER_HXX
#define GAUSSIANSOLVER_HXX

#include <string>
#include <vector>

#include "Solver.def.hxx"

//Foward declaration
class Matrix;

template <class TSource, class TResult>
class GaussianSolver<TSource, TResult> :
    Solver<TSource, TResult>{
private:
    TSource *_matA=NULL, *_matB=NULL, *_opResult=NULL;

    std::string _failMessage;

    double CalculateResultVectorRow(const TSource*, const uint) const;
    void FindAndSwapRowDiagonalMax(const uint);
	void MultiplyRowByScalar(const uint, const double);
	void ReplaceInputParameter(TSource*&, TSource*);
	void SetBelowDiaginalRowToZero(const uint);
	void SetRowDiagonalToOne(const uint);
	void SubtractScalarRowFromRow(const uint, const uint);

public:
    GaussianSolver();
    GaussianSolver(const GaussianSolver&);
    ~GaussianSolver();
    GaussianSolver& operator=(const GaussianSolver&);

    std::string GetFailMessage() const;
    Matrix* GetInput() const;
    Matrix* GetResult() const;

    void CalculateBackSolveVector();
    void ConvertInputToUpperRowEchilon();
    void Clear();
    void SetInput(Matrix*, GaussianSolver::InputParameter);
    bool SolveCompletely();
    double TwoNormOfError();
};
#endif
