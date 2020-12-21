#include <cmath>
#include <ostream>

#include "Matrix.hxx"
#include "PowerSolver.hxx"
#include "Utilities.hxx"

#ifndef ENABLETRACE_POWERSOLVER
    #ifdef TRACEPOWER
        #define ENABLETRACE_POWERSOLVER true
    #else
        #define ENABLETRACE_POWERSOLVER false
    #endif
#endif
#ifndef METHODHEADER_POWERSOLVER
    #define METHODHEADER_POWERSOLVER Tracer _t_(__PRETTY_FUNCTION__, ENABLETRACE_POWERSOLVER)
#endif

using namespace std;

//Constructors/Destructors --------------------------------------
PowerSolver::PowerSolver(const PowerSolver& other){
    METHODHEADER_POWERSOLVER;
    *this = other;
}

PowerSolver& PowerSolver::operator=(PowerSolver rhs){
    METHODHEADER_POWERSOLVER;
    if (this != &rhs){
        swap(_A, rhs._A);
        swap(_result, rhs._result);
    }
    return *this;

}

PowerSolver::~PowerSolver(){
    METHODHEADER_POWERSOLVER;
    Clear();
}

//Methods -------------------------------------------------------
void PowerSolver::AddDataElement(Solver::DataElement param, Matrix* m){
    METHODHEADER_POWERSOLVER;
    switch(param){
        case Solver::DataElement::MatrixA:
            _A = m->DeepCopy();
            break;
         default:
            throw invalid_argument(_INVALID_DATA_ELEMENT_ADD_MSG);
    }
}

//Calculate the Rayleigh Quotiant
double PowerSolver::CalculateDominateEigenValue(const Matrix* bOld, const Matrix* bNew){
    double result = 0.0;
    double sqrdSumOfOldValues = 0.0;

    for(uint r = 0; r < bOld->RowCount(); r++){
        result += (*bOld)(r,0) * (*bNew)(r,0);
        sqrdSumOfOldValues += pow((*bOld)(r,0),2);
    }

    result /= sqrdSumOfOldValues;

     return result;
}

Matrix* PowerSolver::CalculateIterationResult(const Matrix* b) const{
    METHODHEADER_POWERSOLVER;
    Matrix* result = MatrixOperations::Multiply(_A, b);
    return result;
}

void PowerSolver::Clear(){
    METHODHEADER_POWERSOLVER;
    delete _A;
    delete _result;
}

Matrix* PowerSolver::GetDataElement(Solver::DataElement param){
    METHODHEADER_POWERSOLVER;
    switch(param){
        case Solver::DataElement::MatrixA:
            return _A->DeepCopy(); break;
        case Solver::DataElement::Result:
            return _result->DeepCopy(); break;
        default:
            throw invalid_argument(_INVALID_DATA_ELEMENT_GET_MSG);
    }
}

void PowerSolver::IterateUntilSolved(){
    METHODHEADER_POWERSOLVER;
    if (_A == NULL)
        throw invalid_argument("Matrix A not set.");

    _result = Matrix::CreateRandom(_A->RowCount(),1);
    bool done = false;
    do{
        Matrix* newResult = CalculateIterationResult(_result);
        _resultEigen = CalculateDominateEigenValue(_result, newResult);

        //Normalize
        double normalizer = newResult->MaxOrMinValueInColumn(0,true);
        newResult = MatrixOperations::MultiplyByScalar(newResult, (1/normalizer));

        done = ((*_result)==(*newResult));

#ifdef DEBUG
    PrintDebug(cerr,"Print [DEBUG] block ...");
    cerr << "\nOld Result: " << endl;
    _result->PrintPretty(cerr);
    cerr << "\nNew Result: " << endl;
    newResult->PrintPretty(cerr);
    cerr << "\nAre both equal? " << done << endl;
    PrintDebug(cerr, "End [DEBUG] block ...");
#endif

        swap(_result, newResult);
        delete newResult;
        _iterationCount++;
    }while(!done);
}

void PowerSolver::PrintPrettyResults(ostream& out)const{
    METHODHEADER_POWERSOLVER;

    out << "\nPower Solver Results:" << endl;
    out << "\nEigen Vector: " << endl;
    _result->PrintPretty(out);
    out << "\nEigen Value: " << GetEigenValue() << endl;
    out << "Number of iterations: " << _iterationCount << endl;

}

bool PowerSolver::SolveCompletely(){
    METHODHEADER_POWERSOLVER;
    bool result = false;
    try{
        IterateUntilSolved();
        result = true;
    }catch(exception ex){
        SetFailMessage(ex);
    }
    return result;
}
