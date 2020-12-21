#ifndef EULERSOLVER_HXX
#define EULERSOLVER_HXX

#include <functional>
#include <iosfwd>

#include "SingleParamDelegateSolver.hxx"
#include "Utilities.hxx"

class EulerSolver:
    public SingleParamDelegateSolver<double, double, double>{
private:
    std::function<double(double)> _fx=NULL;
    std::function<double(double,double)> _fx_d_d=NULL;
    std::experimental::optional<double> _actY, _currdydx,
        _currX, _currY, _sampleInterval, _startingY, _targetX;
    int _iterationCount = 0;

    template <typename T>
    void PrintColumnOutput(std::ostream&, T) const;

public:
    EulerSolver(){}
    EulerSolver(const EulerSolver&);
    EulerSolver& operator=(const EulerSolver&);
    virtual ~EulerSolver();

    void AddDelegateElement(SingleParamDelegateSolver::DelegateElement, std::function<double(double)>);
    void AddDelegateElement(SingleParamDelegateSolver::DelegateElement, std::function<double(double,double)>);
    void AddDataElement(Solver::DataElement, double*);
    virtual void Clear();
    double* GetDataElement(Solver::DataElement);
    void IterateUntilSolved();
    Solver::SolverType GetSolverType()const {return Solver::SolverType::Euler;}
    void PrintPrettyResults(std::ostream&)const;
    bool SolveCompletely();
    void ValidateInputs() const;
};


#endif // EULERSOLVER_HXX_INCLUDED
