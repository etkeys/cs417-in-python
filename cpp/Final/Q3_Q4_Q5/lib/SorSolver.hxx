#ifndef SORSOLVER_HXX
#define SORSOLVER_HXX

#include <iosfwd>

#include "GausSeidelSolver.hxx"

class Matrix;

class SorSolver:
    public GausSeidelSolver{
protected:
    Matrix* _omega=NULL;

    Matrix* CalculateIterationResult(const Matrix*);

public:
    SorSolver(){};
    SorSolver(const SorSolver&);
    virtual ~SorSolver();
    SorSolver& operator=(const SorSolver& );

    void AddDataElement(Solver::DataElement, Matrix*);
    virtual void Clear();
    Matrix* GetDataElement(Solver::DataElement);
    Solver::SolverType GetSolverType(){return Solver::SolverType::Sor;}
    void PrintPrettyResults(std::ostream&) const;

};

#endif
