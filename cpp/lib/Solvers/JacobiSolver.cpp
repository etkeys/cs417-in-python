#include <stdexcept>
#if DEBUG
    #include <sstream>
#endif

#include "JacobiSolver.hxx"
#include "Matrix.hxx"
#include "Utilities.hxx"

#ifndef ENABLETRACE_JACOBISOLVER
    #ifdef TRACESOLVER
        #define ENABLETRACE_JACOBISOLVER true
    #else
        #define ENABLETRACE_JACOBISOLVER false
    #endif
#endif
#ifndef METHODHEADER_JACOBI
    #define METHODHEADER_JACOBI Tracer _t_(__PRETTY_FUNCTION__, ENABLETRACE_JACOBISOLVER)
#endif

using namespace std;

JacobiSolver::JacobiSolver(){
    METHODHEADER_JACOBI;
    _iterCount = new Matrix(1);
    (*_iterCount)(0,0) = 0.0;
}

JacobiSolver::JacobiSolver(const JacobiSolver& other){
    METHODHEADER_JACOBI;
    *this = other;
}

JacobiSolver& JacobiSolver::operator=(const JacobiSolver& other){
    METHODHEADER_JACOBI;
    if (this != &other){
        Clear();
        _iterCount = other._iterCount;
        _A = other._A->DeepCopy();
        _b = other._b->DeepCopy();
        _C = other._C->DeepCopy();
        _D = other._D->DeepCopy();
        _result = other._D->DeepCopy();
    }
    return *this;
}

JacobiSolver::~JacobiSolver(){
    METHODHEADER_JACOBI;
    Clear();
}

void JacobiSolver::AddDataElement(Solver::DataElement param, Matrix* m){
    METHODHEADER_JACOBI;
    switch(param){
        case Solver::DataElement::MatrixA:
            delete _A; _A = m->DeepCopy(); break;
        case Solver::DataElement::Matrixb:
            delete _b; _b = m->DeepCopy(); break;
        default:
            throw invalid_argument(_INVALID_DATA_ELEMENT_ADD_MSG);
    }
}

Matrix* JacobiSolver::CalculateIterationResult(const Matrix* guess){
    METHODHEADER_JACOBI;
    Matrix* cg = MatrixOperations::Multiply(_C, guess);
    Matrix* bcg = MatrixOperations::Subtract(_b, cg);
    Matrix* result = MatrixOperations::Multiply(_D, bcg);
    (*_iterCount)(0,0)++;

    delete cg;
    delete bcg;
    return result;
}

void JacobiSolver::Clear(){
    METHODHEADER_JACOBI;
    delete _A;
    delete _b;
    delete _C;
    delete _D;
    delete _result;
}

void JacobiSolver::CreateInterimMatrices(){
    METHODHEADER_JACOBI;
    CreateCMatrix();
    CreateDMatrix();
}

void JacobiSolver::CreateCMatrix(){
    METHODHEADER_JACOBI;
    if (_A == NULL)
        throw invalid_argument("Matrix A has not been provided.");

    _C = Matrix::CreateNonDiagonalTermMatrix(_A);
}

void JacobiSolver::CreateDMatrix(){
    METHODHEADER_JACOBI;
    if (_A == NULL)
        throw invalid_argument("Matrix A has not been provided.");

    _D = Matrix::CreateDiagonalTermMatrix(_A);
    _D->Invert();
}

Matrix* JacobiSolver::GetDataElement(Solver::DataElement param){
    METHODHEADER_JACOBI;
    switch(param){
        case Solver::DataElement::IterationCount:
            return _iterCount->DeepCopy();
        case Solver::DataElement::MatrixA:
            return _A->DeepCopy();
        case Solver::DataElement::Matrixb:
            return _b->DeepCopy();
        case Solver::DataElement::MatrixC:
            return _C->DeepCopy();
        case Solver::DataElement::MatrixD:
            return _D->DeepCopy();
        case Solver::DataElement::Result:
            return _result->DeepCopy();
        default:
            throw invalid_argument(_INVALID_DATA_ELEMENT_GET_MSG);
    }
}

int JacobiSolver::GetIterationCount(){
    METHODHEADER_JACOBI;
    return (int)(*_iterCount)(0,0);
}

int JacobiSolver::GetIterationCount() const{
    METHODHEADER_JACOBI;
    return (int)(*_iterCount)(0,0);
}

void JacobiSolver::IterateUntilSolved(){
    METHODHEADER_JACOBI;
    ValidateMatrices();

    IterateUntilSolved(false);
}

void JacobiSolver::IterateUntilSolved(bool useCForGuess){
    METHODHEADER_JACOBI;
    const int REPEATEDRESULTLIMIT = 1000;
    double twoNorm, prevTwoNorm = 0.0;
    int repeatedResult = 0;

    if (useCForGuess)
        _result = _C->DeepCopy();
    else
        _result = Matrix::CreateRandom(_C->RowCount(), 1);


    do{
        Matrix* guess = _result;
        _result = CalculateIterationResult(guess);
        twoNorm = MatrixOperations::TwoNormOfError(_A, _b, _result);
#ifdef DEBUG
    PrintDebug(cout, "Printing [DEBUG] block");

    cout << "\n Current result: " << endl;
    _result->PrintPretty(cout);
    cout << "\nCurrent two norm: " << twoNorm
        << "\nPrevious two norm: " << prevTwoNorm
        << "\nRepreated Results: " << repeatedResult << endl;

    PrintDebug(cout, "... Done printing [DEBUG] block.");
#endif
        if (DoubleEquals(twoNorm, prevTwoNorm)){
            repeatedResult++;
        }else{
            prevTwoNorm = twoNorm;
            repeatedResult = 0;
        }

        delete guess;
    }while (!DoubleEquals(twoNorm, 0.0) && repeatedResult < REPEATEDRESULTLIMIT);

    if (repeatedResult >= REPEATEDRESULTLIMIT)
        _lameConverg = true;
}

void JacobiSolver::PrintPrettyResults(ostream& out) const{
    METHODHEADER_JACOBI;

    PrintPrettyResults(out, "Jacobi Iterative Solver Results:");
}

void JacobiSolver::PrintPrettyResults(ostream& out, string header)const{
    METHODHEADER_JACOBI;

   out << header
        << "\nMatrix A:" << endl;
    _A->PrintPretty(out);

    out    << "\nMatrix b (right-hand-side):" << endl;
    _b->PrintPretty(out);

    out << "\nResult Matrix:" << endl;
    _result->PrintPretty(out);

    if (_lameConverg)
        out << "\nWARNING: Lame convergence was detected" << endl;

    out << "\nTwo norm of the error: "
        << MatrixOperations::TwoNormOfError(_A,_b,_result) << endl;

    out << "Number of iterations: " << GetIterationCount() << endl;
}

bool JacobiSolver::SolveCompletely(){
    METHODHEADER_JACOBI;
    bool result = false;
    try{
        CreateInterimMatrices();
        IterateUntilSolved();
        result = true;
    }catch (exception ex){
        SetFailMessage(ex);
    }

    return result;
}

void JacobiSolver::ValidateMatrices(){
    METHODHEADER_JACOBI;
    if (_C == NULL || _D == NULL || !_D->IsInverted())
        throw invalid_argument("Either Matrix C or D is missing or D is not in the correct form.");
}
