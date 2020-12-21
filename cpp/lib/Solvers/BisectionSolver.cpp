#include <iostream>
#if DEBUG
    #include <sstream>
#endif

#include "BisectionSolver.hxx"
#include "Utilities.hxx"

#ifndef ENABLETRACE_BISECTIONSOLVER
    #ifdef TRACEBISECTION
        #define ENABLETRACE_BISECTIONSOLVER true
    #else
        #define ENABLETRACE_BISECTIONSOLVER false
    #endif
#endif
#ifndef METHODHEADER_BISECTION
    #define METHODHEADER_BISECTION Tracer _t_(__PRETTY_FUNCTION__, ENABLETRACE_BISECTIONSOLVER)
#endif

using namespace std;

BisectionSolver::BisectionSolver(const BisectionSolver& other){
    METHODHEADER_BISECTION;
    *this=other;
}

BisectionSolver& BisectionSolver::operator=(const BisectionSolver& other){
    METHODHEADER_BISECTION;
    if (this != &other){
        Clear();

        _accuracy = other._accuracy;
        _end0 = other._end0;
        _end1 = other._end1;
        _fx = other._fx;
        _iterationCount = other._iterationCount;
        _result = other._result;
    }
    return *this;
}

BisectionSolver::~BisectionSolver(){
    METHODHEADER_BISECTION;
    Clear();
}

void BisectionSolver::AddDelegateElement(SingleParamDelegateSolver::DelegateElement param,
                        function<double(double)> f){
    METHODHEADER_BISECTION;
    switch(param){
        case SingleParamDelegateSolver::DelegateElement::FofX:
            _fx = f; break;
        default:
            throw invalid_argument(_INVALID_DATA_ELEMENT_ADD_MSG);
    }

}

void BisectionSolver::AddDataElement(Solver::DataElement param, double* d){
    METHODHEADER_BISECTION;
    switch(param){
        case Solver::DataElement::BisectionAccuracy:
            _accuracy=(*d); break;
        case Solver::DataElement::BisectionEnd1:
            _end0 = (*d); break;
        case Solver::DataElement::BisectionEnd2:
            _end1 = (*d); break;
        default:
            throw invalid_argument(_INVALID_DATA_ELEMENT_ADD_MSG);
    }

}

void BisectionSolver::Clear(){
    METHODHEADER_BISECTION;
    _fx=NULL;
    _end0.reset();
    _end1.reset();
    _result = 0.0;
    _iterationCount = 0.0;
}

double* BisectionSolver::GetDataElement(Solver::DataElement param){
    METHODHEADER_BISECTION;
    switch(param){
        case Solver::DataElement::Result:
            return &(_result.value());
        case Solver::DataElement::IterationCount:
            return (double*)&_iterationCount;
        default:
            throw invalid_argument(_INVALID_DATA_ELEMENT_GET_MSG);
    }
}

void BisectionSolver::IterateUntilSolved(){
    METHODHEADER_BISECTION;
    double middle, resultMiddle, resultRight;
    bool done = false;

    do{
        middle = (_end0.value() + _end1.value())/2;

        resultRight = _fx(_end1.value());
        resultMiddle = _fx(middle);

        //if resultRight and resultMiddle are both positive, move end1 to middle
        //  else move end0 to middle
        if (resultMiddle > 0 && resultRight > 0)
            _end1 = middle;
        else
            _end0 = middle;

        done = DoubleEquals(resultMiddle, 0.0, _accuracy.value());

        _iterationCount++;

#if DEBUG
        stringstream ss;
        ss << "End0=" << _end0.value() << ";End1=" << _end1.value()
            << ";CurrentResult=" << resultMiddle
            << ";CurrentIteration=" << _iterationCount;
        PrintDebug(cerr, ss.str()); ss.clear();
#endif
    }while(!done); //what is true?

    _result = middle;
}

void BisectionSolver::PrintPrettyResults(ostream& out)const{
    METHODHEADER_BISECTION;
    out << "\nBisection Solver Results: " << endl << endl;
    out << "Solution value: " << _result.value() << endl;
    out << "Funtion reslt: " << _fx(_result.value()) << endl;
    out << "Number of iterations: " << _iterationCount << endl;
    out << endl;
}

bool BisectionSolver::SolveCompletely(){
    METHODHEADER_BISECTION;
    bool result = false;
    try{
        ValidateFX();
        ValidateEndPoints();
        IterateUntilSolved();
        result = true;
    }catch (exception ex){
        SetFailMessage(ex);
    }
    return result;
}

void BisectionSolver::ValidateEndPoints(){
    if (!_end0 || !_end1)
        throw invalid_argument("Either endpoint 1 or endpoint 2 is not set.");
}

void BisectionSolver::ValidateFX(){
    if (_fx == NULL)
        throw invalid_argument("Function f(x) not provided.");
}
