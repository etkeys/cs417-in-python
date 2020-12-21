#ifndef BISECTIONSOLVER_HXX
#define BISECTIONSOLVER_HXX

#include <functional>
#include <iosfwd>

#include "SingleParamDelegateSolver.hxx"
#include "Utilities.hxx"

/*
template <typename T>
class Nullable<T>;
*/

class BisectionSolver:
    public SingleParamDelegateSolver<double, double, double>{
private:
    std::function<double(double)> _fx=NULL;
    //Nullable<double> _result, _end0, _end1, _accuracy;
    std::experimental::optional<double> _result, _end0, _end1, _accuracy;
    int _iterationCount=0;

    void AddDelegateElement(SingleParamDelegateSolver::DelegateElement, std::function<double(double,double)>){}

public:
    BisectionSolver(){}
    BisectionSolver(const BisectionSolver&);
    BisectionSolver& operator=(const BisectionSolver&);
    virtual ~BisectionSolver();

    void AddDelegateElement(SingleParamDelegateSolver::DelegateElement, std::function<double(double)>);
    void AddDataElement(Solver::DataElement, double*);
    void Clear();
    double* GetDataElement(Solver::DataElement);
    Solver::SolverType GetSolverType()const{return Solver::SolverType::Bisection;}
    void PrintPrettyResults(std::ostream&)const;
    void IterateUntilSolved();
    bool SolveCompletely();
    void ValidateEndPoints();
    void ValidateFX();
};

#endif
