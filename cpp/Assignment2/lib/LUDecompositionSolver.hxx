#ifndef LUDECOMPOSITIONSOLVER_HXX
#define LUDECOMPOSITIONSOLVER_HXX

#include <iosfwd>

#include "Solver.def.hxx"

class Matrix; //Forward declaration

class LuSolver:
    public Solver<Matrix, Matrix>{
private:
    Matrix *_A=NULL, *_b=NULL, *_L=NULL,*_U=NULL,
            *_y=NULL, *_result=NULL;

    void CalculateColumnForL(const uint, const uint);
    void CalculateRowForU(const uint, const uint);
    double CalculateYVectorRow(const uint, const Matrix*) const;
    void InitializeCroutL();
    void InitializeCroutU();

public:
    LuSolver(){}
    LuSolver(const LuSolver&);
    virtual ~LuSolver();
    LuSolver& operator=(const LuSolver&);

    virtual void AddDataElement(Solver::DataElement, Matrix*);
    void Clear();
    void CalculateBackSolveVector();
    void CalculateForwardSolveVector();
    void CroutFactoringAlgorithm();
    virtual Matrix* GetDataElement(Solver::DataElement);
    virtual Solver::SolverType GetSolverType() const {return Solver::SolverType::LUDecomposition;}
    void PrintPrettyResults(std::ostream&)const;
    virtual bool SolveCompletely();
};

#endif // LUDECOMPOSITIONSOLVER_HXX
