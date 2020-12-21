#ifndef NEWTONSOLVER_HXX
#define NEWTONSOLVER_HXX

#include <iosfwd>

#include "SingleParamDelegateSolver.hxx"

class NewtonSolver:
    public SingleParamDelegateSolver<double,double>{
private:
    std::function<double(double)> _fx = NULL, _fprimex = NULL;
    double _result, _iterationCount = 0.0;
    bool _convergeFail = false;

public:
    NewtonSolver(){};
    NewtonSolver(const NewtonSolver&);
    NewtonSolver& operator=(const NewtonSolver&);
    ~NewtonSolver();

    void AddDataElement(Solver::DataElement, double* item);
    void AddDelegateElement(SingleParamDelegateSolver::DelegateElement param,
                            std::function<double(double)>);
    virtual void Clear();
    double DelegateInvoke(SingleParamDelegateSolver::DelegateElement, double);
    virtual double* GetDataElement(Solver::DataElement);
    Solver::SolverType GetSolverType()const{return Solver::SolverType::Newton;}
    void IterateUntilSolved();
    void PrintPrettyResults(std::ostream&) const;
    bool SolveCompletely();

};

#endif
