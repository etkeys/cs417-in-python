#ifndef SOLVER_DEF_HXX
#define SOLVER_DEF_HXX

#include <exception>
#include <string>
#include "Utilities.hxx"

template <class TSource, class TResult>
class Solver{
public: enum DataElement {BisectionAccuracy, BisectionEnd1, BisectionEnd2, EulerDyDx,
                            EulerSamplingInterval, EulerStartingY, EulerTargetX,
                            IterationCount, MatrixA, Matrixb, MatrixC, MatrixD,
                            MatrixL, MatrixU, Matrixy, NewtonIterations,
                            SorOmega, Result};
public: enum SolverType {Bisection, Euler, GausSeidel, Gaussian, Jacobi,
                            LUDecomposition, Newton, Power, Sor, Unknown};

protected:
    const std::string _INVALID_DATA_ELEMENT_ADD_MSG = "Either the requested data element cannot be set in this manner or is not valid in this context.";
    const std::string _INVALID_DATA_ELEMENT_GET_MSG = "Requested data element is not valid in this context";
    std::string _failMessage;

    void ReplaceDataElement(TSource*&, TSource*);
    void SetFailMessage(std::exception ex){
        _failMessage = "Error during processing.\nwhat(): " + std::string(ex.what());
    }

public:
    virtual ~Solver(){}

    virtual void AddDataElement(Solver::DataElement, TSource* ) = 0;
    virtual void Clear() = 0;
    virtual std::string GetFailMessage() const {return _failMessage;}
    virtual TResult* GetDataElement(Solver::DataElement) = 0;
    virtual Solver::SolverType GetSolverType() const = 0;
    virtual bool SolveCompletely() = 0;

};

#endif // SOLVER_DEF_HXX
