#include <iomanip>
#include <iostream>

#include "EulerSolver.hxx"
#include "Utilities.hxx"

#ifndef ENABLETRACE_EULERSOLVER
    #ifdef TRACEEULER
        #define ENABLETRACE_EULERSOLVER true
    #else
        #define ENABLETRACE_EULERSOLVER false
    #endif
#endif
#ifndef METHODHEADER_EULER
    #define METHODHEADER_EULER Tracer _t_(__PRETTY_FUNCTION__, ENABLETRACE_EULERSOLVER)
#endif

using namespace std;
using namespace std::experimental;

EulerSolver::EulerSolver(const EulerSolver& other){
    METHODHEADER_EULER;
    *this = other;
}

EulerSolver& EulerSolver::operator=(const EulerSolver& other){
    METHODHEADER_EULER;
    if (this != &other){
        Clear();
        _actY = other._actY;
        _currdydx = other._currdydx;
        _currX = other._currX;
        _currY = other._currY;
        _iterationCount = other._iterationCount;
        _sampleInterval = other._sampleInterval;
        _startingY = other._startingY;
        _targetX = other._targetX;

    }
    return *this;
}

EulerSolver::~EulerSolver(){
    METHODHEADER_EULER;
    Clear();
}

void EulerSolver::AddDelegateElement(SingleParamDelegateSolver::DelegateElement param,
                                    std::function<double(double)> f){
    METHODHEADER_EULER;
    switch(param){
        case SingleParamDelegateSolver::DelegateElement::FofX:
            _fx = f; break;
        default:
            throw invalid_argument(_INVALID_DATA_ELEMENT_ADD_MSG);
    }
}

void EulerSolver::AddDelegateElement(SingleParamDelegateSolver::DelegateElement param,
                                    std::function<double(double,double)> f){
    METHODHEADER_EULER;
    switch(param){
        case SingleParamDelegateSolver::DelegateElement::FofX:
            _fx_d_d = f; break;
        default:
            throw invalid_argument(_INVALID_DATA_ELEMENT_ADD_MSG);
    }
}

void EulerSolver::AddDataElement(Solver::DataElement param, double* d){
    METHODHEADER_EULER;
    switch(param){
        case Solver::DataElement::EulerTargetX:
            _targetX = (*d); break;
        case Solver::DataElement::EulerSamplingInterval:
            _sampleInterval = (*d); break;
        case Solver::DataElement::EulerStartingY:
            _startingY = (*d);break;
        default:
            throw invalid_argument(_INVALID_DATA_ELEMENT_ADD_MSG);

    }
}

void EulerSolver::Clear(){
    METHODHEADER_EULER;
    _targetX.reset();
    _sampleInterval.reset();
    _startingY.reset();
    _currX.reset();
    _currY.reset();
    _currdydx.reset();
    _actY.reset();
}

double* EulerSolver::GetDataElement(Solver::DataElement param){
    METHODHEADER_EULER;
    double* result;
    switch(param){
        case Solver::DataElement::EulerSamplingInterval:
            result = _sampleInterval ? &(_sampleInterval.value()) : nullptr; break;
        case Solver::DataElement::EulerStartingY:
            result = _startingY ? &(_startingY.value()) : nullptr; break;
        case Solver::DataElement::EulerTargetX:
            result = _targetX ? &(_targetX.value()) : nullptr; break;
        case Solver::DataElement::EulerDyDx:
            result = _currdydx ? &(_currdydx.value()) : nullptr; break;
        case Solver::DataElement::Result:
            result = _actY ? &(_currdydx.value()): nullptr; break;
        case Solver::DataElement::IterationCount:
            result = (double*)&_iterationCount;
        default:
            throw invalid_argument(_INVALID_DATA_ELEMENT_GET_MSG);
    }
    return result;
}

void EulerSolver::IterateUntilSolved(){
    METHODHEADER_EULER;
    ValidateInputs();
    _currX = 0.0;
    //_currdydx = _startingY;
    _currY = _startingY;

    do{
        //_currdydx = _currdydx.value() + (_sampleInterval.value() * _currdydx.value());
        //_actY = _fx_d_d(_currdydx.value(), _currX.value());
        _currdydx = _fx_d_d(_currY.value(), _currX.value());
        _actY = _currY.value() + (_sampleInterval.value() * _currdydx.value());
        _iterationCount++;
        PrintPrettyResults(cout); //BAD PRACTICE...If this were real, we wouldn't pass cout directly
        _currY = _actY;
        _currX = _currX.value() + _sampleInterval.value();
    }while(_currX.value() + _sampleInterval.value() <= _targetX.value());
}

template <typename T>
void EulerSolver::PrintColumnOutput(ostream& out, T val)const{
    //METHODHEADER_EULER;
    out << left << setprecision(7) << setw(13) << val;
}

void EulerSolver::PrintPrettyResults(std::ostream& out)const{
    METHODHEADER_EULER;
    std::streamsize ssize = out.precision();
    //auto columnOutput = []<typename T>(ostream& out, T val){ out << left << setprecision(7)<< setw(10) << val;};

    out << "\nEuler Solver Results:\n" << endl;

    //PrintHeader
    PrintColumnOutput(out, "TargetX");
    PrintColumnOutput(out, "X");
    PrintColumnOutput(out, "Y");
    PrintColumnOutput(out, "dy/dx");
    PrintColumnOutput(out, "actual_Y");
    out << endl;

    //print data
    _targetX ? PrintColumnOutput(out, _targetX.value()) : PrintColumnOutput(out, "NULL");
    _currX ? PrintColumnOutput(out, _currX.value()) : PrintColumnOutput(out, "NULL");
    _currY ? PrintColumnOutput(out, _currY.value()) : PrintColumnOutput(out, "NULL");
    _currdydx ? PrintColumnOutput(out, _currdydx.value()) : PrintColumnOutput(out, "NULL");
    _actY ? PrintColumnOutput(out, _actY.value()) : PrintColumnOutput(out, "NULL");
    out << endl;

    out << "Number of iterations: " << _iterationCount << endl;

    out << setprecision(ssize);
}

bool EulerSolver::SolveCompletely(){
    METHODHEADER_EULER;
    bool result = false;
    try{
        IterateUntilSolved();
        result = true;
    }catch(exception ex){
        SetFailMessage(ex);
    }
    return result;
}

void EulerSolver::ValidateInputs()const{
    METHODHEADER_EULER;
    if (!_targetX || !_sampleInterval || !_startingY)
        throw invalid_argument("TargetX, SampleInterval, or StartingY not set");
}
