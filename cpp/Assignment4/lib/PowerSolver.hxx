#ifndef POWERSOLVER_HXX
#define POWERSOLVER_HXX

#include "Solver.def.hxx"

class Matrix;

class PowerSolver:
    public Solver<Matrix,Matrix>{
private:
    Matrix *_A=NULL, *_result=NULL;
    double _resultEigen = 0.0;
    int _iterationCount = 0;

    double CalculateDominateEigenValue(const Matrix*, const Matrix*);
    Matrix* CalculateIterationResult(const Matrix*) const;

public:
    PowerSolver(){}
    PowerSolver(const PowerSolver&);
    PowerSolver& operator=(PowerSolver);
    virtual ~PowerSolver();

    virtual void AddDataElement(Solver::DataElement, Matrix*);
    virtual void Clear();
    virtual Matrix* GetDataElement(Solver::DataElement);
    double GetEigenValue() const {return _resultEigen;}
    void IterateUntilSolved();
    void PrintPrettyResults(std::ostream&) const;
    Solver::SolverType GetSolverType()const{return Solver::SolverType::Power;}
    virtual bool SolveCompletely();

};


#endif // POWERSOLVER_HXX
