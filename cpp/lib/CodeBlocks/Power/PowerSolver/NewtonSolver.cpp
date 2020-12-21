#include <cstdlib>
#include <ctime>
#include <functional>
#include <string>

#include "NewtonSolver.hxx"
#include "Utilities.hxx"

#ifndef ENABLETRACE_NEWTONSOLVER
    #ifdef TRACENEWTON
        #define ENABLETRACE_NEWTONSOLVER true
    #else
        #define ENABLETRACE_NEWTONSOLVER false
    #endif
#endif
#ifndef METHODHEADER_NEWTONSOLVER
    #define METHODHEADER_NEWTONSOLVER Tracer _t_(__PRETTY_FUNCTION__, ENABLETRACE_NEWTONSOLVER)
#endif
#ifdef DEBUG
    #include <sstream>
#endif

using namespace std;

NewtonSolver::NewtonSolver(const NewtonSolver& other){
    METHODHEADER_NEWTONSOLVER;
    *this = other;
}
NewtonSolver& NewtonSolver::operator=(const NewtonSolver& other){
    METHODHEADER_NEWTONSOLVER;
    if (this == &other){
        Clear();

        _fx = other._fx;
        _fprimex = other._fprimex;
        _result = other._result;
    }
    return *this;
}
NewtonSolver::~NewtonSolver(){
    METHODHEADER_NEWTONSOLVER;
    Clear();
};

void NewtonSolver::AddDataElement(Solver::DataElement param, double* item){
    METHODHEADER_NEWTONSOLVER;
    switch(param){
        default:
            throw invalid_argument(_INVALID_DATA_ELEMENT_ADD_MSG);
    }
}

void NewtonSolver::AddDelegateElement(SingleParamDelegateSolver::DelegateElement param,
                                        function<double(double)> f){
    METHODHEADER_NEWTONSOLVER;
    switch(param){
        case SingleParamDelegateSolver::DelegateElement::FofX:
            _fx = f;break;
        case SingleParamDelegateSolver::DelegateElement::FPrimeofX:
            _fprimex = f;break;
        default:
            throw invalid_argument(_INVALID_DATA_ELEMENT_ADD_MSG);
    }
}

void NewtonSolver::Clear(){
    METHODHEADER_NEWTONSOLVER;
    _fx = NULL;
    _fprimex = NULL;
    _result = 0.0;
}

double NewtonSolver::DelegateInvoke(SingleParamDelegateSolver::DelegateElement func,
                                    double param){
    METHODHEADER_NEWTONSOLVER;
    switch(func){
        case SingleParamDelegateSolver::DelegateElement::FofX:
            return _fx(param);
        case SingleParamDelegateSolver::DelegateElement::FPrimeofX:
            return _fprimex(param);
        default:
            throw invalid_argument(_INVALID_DATA_ELEMENT_GET_MSG);
    }
}

double* NewtonSolver::GetDataElement(Solver::DataElement param){
    METHODHEADER_NEWTONSOLVER;
    switch(param){
        case Solver::DataElement::Result:
            return &_result;
        case Solver::DataElement::NewtonIterations:
            return &_iterationCount;
        default:
            throw invalid_argument(_INVALID_DATA_ELEMENT_GET_MSG);
    }
}

void NewtonSolver::IterateUntilSolved(){
    METHODHEADER_NEWTONSOLVER;
    bool done = false;
    double xOld;

    srand(time(NULL));
    xOld = rand() % 100 + 1;
    do{
        double rprime = DelegateInvoke(SingleParamDelegateSolver::DelegateElement::FPrimeofX, xOld);
        double r = DelegateInvoke(SingleParamDelegateSolver::DelegateElement::FofX, xOld);

        done = DoubleEquals(rprime, 0.0);

        if (done)
            _convergeFail = true;
        else{
            double xNew = xOld - (r/rprime);

#ifdef DEBUG
            stringstream ss;
            ss << "r=" << r <<  ";rprime=" << rprime
                << ";OldValue=" << xOld << ";NewValue=" << xNew;
            PrintDebug(cerr, ss.str());
#endif

            done = DoubleEquals(xOld, xNew);
            xOld = xNew;
            _iterationCount++;
        }
    }while(!done);

    _result = xOld;
}

void NewtonSolver::PrintPrettyResults(ostream& out)const{
    METHODHEADER_NEWTONSOLVER;
    out << "\nNewton Solver Results: " <<endl;
    out << "\nSolution: ";

    if (_convergeFail)
        out << "Divergent. No result.";
    else
        out << _result;

    out << endl;

    out << "Number of iterations: " << _iterationCount << endl << endl;

}

bool NewtonSolver::SolveCompletely(){
    METHODHEADER_NEWTONSOLVER;
    bool result = false;
    try{
        IterateUntilSolved();
        result = true;
    }catch(exception ex){
        SetFailMessage(ex);
    }
    return result;
}
