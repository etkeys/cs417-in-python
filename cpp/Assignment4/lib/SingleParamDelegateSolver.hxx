#ifndef SINGLEPARAMDELEGATESOLVER_HXX
#define SINGLEPARAMDELEGATESOLVER_HXX

#include <functional>
#include "Solver.def.hxx"

//Use of the function element from:
//http://www.cplusplus.com/reference/functional/function/function/
//http://www.cplusplus.com/forum/beginner/196638/

template <class T1, class TResult>
class SingleParamDelegateSolver:
    public Solver<double, double>{
public: enum DelegateElement {FofX, FPrimeofX};

public:

    virtual void AddDelegateElement(SingleParamDelegateSolver::DelegateElement,
                                    std::function<TResult(T1)>) = 0;


};

#endif // SINGLEPARAMDELEGATESOLVER_HXX
