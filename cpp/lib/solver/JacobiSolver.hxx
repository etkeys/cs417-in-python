#ifndef JACOBISOLVER_HXX
#define JACOBISOLVER_HXX

#include <iosfwd>
#include <string>

#include "Solver.def.hxx"

class Matrix;

//Used to solve problems in the form of
//  x = (D^-1)(b-Cx)

class JacobiSolver:
    public Solver<Matrix,Matrix>{
protected:
    Matrix *_iterCount;
    Matrix *_A=NULL, *_b=NULL, *_C=NULL, *_D=NULL, *_result=NULL;
    bool _lameConverg = false;

    virtual Matrix* CalculateIterationResult(const Matrix*);
    virtual void IterateUntilSolved(bool);
    virtual void PrintPrettyResults(std::ostream&, std::string) const;

public:
    JacobiSolver();
    JacobiSolver(const JacobiSolver&);
    virtual ~JacobiSolver();
    JacobiSolver& operator=(const JacobiSolver&);

    virtual void AddDataElement(Solver::DataElement, Matrix*);
    virtual void Clear();
    virtual void CreateInterimMatrices();
    void CreateCMatrix();
    void CreateDMatrix();
    virtual Matrix* GetDataElement(Solver::DataElement);
    virtual int GetIterationCount();
    virtual int GetIterationCount() const;
    bool HasLameConvergence() const {return _lameConverg;}
    virtual void IterateUntilSolved();
    virtual Solver::SolverType GetSolverType() const {return Solver::SolverType::Jacobi;}
    void PrintPrettyResults(std::ostream&) const;
    virtual bool SolveCompletely();
    virtual void ValidateMatrices();
};

#endif // JACOBISOLVER_HXX
