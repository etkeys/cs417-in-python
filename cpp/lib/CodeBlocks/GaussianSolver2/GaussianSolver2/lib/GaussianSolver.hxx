#ifndef GAUSSIANSOLVER_H
#define GAUSSIANSOLVER_H

#include <string>
#include <vector>

#include "Solver.def.hxx"

//Foward declaration
class Matrix;

class GaussianSolver :
    public Solver<Matrix, Matrix>{
private:
    Matrix *_matA=NULL, *_matB=NULL, *_opResult=NULL;

    double CalculateResultVectorRow(const Matrix*, const uint) const;

public:
    GaussianSolver(){}
    GaussianSolver(const GaussianSolver&);
    virtual ~GaussianSolver();
    GaussianSolver& operator=(const GaussianSolver&);

    virtual void AddDataElement(Solver::DataElement, Matrix*);
    virtual Matrix* GetDataElement(Solver::DataElement);

    void CalculateBackSolveVector();;
    virtual Solver::SolverType GetSolverType() const {return Solver::SolverType::Gaussian;}
    virtual void Clear();
    virtual bool SolveCompletely();
};
#endif
