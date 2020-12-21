#ifndef GAUSSEIDELSOLVER_HXX
#define GAUSSEIDELSOLVER_HXX

#include "JacobiSolver.hxx"

class Matrix; //Forward declare

//Used to solve problems in the form of:
//  x^(k+1) = (Dx^k)+C
//  where:
//      D = (-L^-1)U; U is strict upper triangluar
//      C = (L^-1)b

class GausSeidelSolver:
    public JacobiSolver{
protected:
    Matrix *_L=NULL, *_U=NULL;
    Matrix* CalculateIterationResult(const Matrix*);

public:
    GausSeidelSolver();
    GausSeidelSolver(const GausSeidelSolver&);
    virtual ~GausSeidelSolver();
    GausSeidelSolver& operator=(const GausSeidelSolver&);


    virtual void AddDataElement(Solver::DataElement, Matrix*);
    virtual void Clear();
    virtual void CreateInterimMatrices();
    void CreateCMatrix();
    void CreateDMatrix();
    void CreateLMatrix();
    void CreateUMatrix();
    virtual Matrix* GetDataElement(Solver::DataElement);
    virtual Solver::SolverType GetSolverType() const {return Solver::SolverType::GausSeidel;}
    void PrintPrettyResults(std::ostream&) const;
    virtual bool SolveCompletely();
    void ValidateMatrices();

};


#endif // GAUSSEIDELSOLVER_HXX
