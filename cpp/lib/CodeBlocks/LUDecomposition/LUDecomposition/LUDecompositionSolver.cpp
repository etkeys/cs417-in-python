/*
* Citation Notes:
* 1: Where this note is referenced, the code is an altered and
*    segmented version found here (Crout Matrix Decomposition):
*   https://en.wikipedia.org/wiki/Crout_matrix_decomposition
*/
#include <stdexcept>

#include "Solver.hxx"
#include "LUDecompositionSolver.hxx"
#include "Matrix.hxx"
#include "Utilities.hxx"

#ifndef ENABLETRACE_LUDECOMPOSITIONSOLVER
    #ifdef TRACESOLVER
        #define ENABLETRACE_LUDECOMPOSITIONSOLVER true
    #else
        #define ENABLETRACE_LUDECOMPOSITIONSOLVER false
    #endif
#endif
#ifndef METHODHEADER_LU
    #define METHODHEADER_LU Tracer _t_(__PRETTY_FUNCTION__, ENABLETRACE_LUDECOMPOSITIONSOLVER)
#endif


using namespace std;

LuSolver::LuSolver(const LuSolver& other){
    METHODHEADER_LU;
    *this=other;
}
LuSolver::~LuSolver(){
    METHODHEADER_LU;
    Clear();
}
LuSolver& LuSolver::operator=(const LuSolver& other){
    METHODHEADER_LU;
    if (this != &other){
        Clear();
        _A = other._A->DeepCopy();
        _b = other._b->DeepCopy();
        _L = other._L->DeepCopy();
        _U = other._U->DeepCopy();
        _y = other._y->DeepCopy();
        _result = other._result->DeepCopy();
    }
    return *this;
}

void LuSolver::AddDataElement(Solver::DataElement param, Matrix* m){
    METHODHEADER_LU;
    switch(param){
        case Solver::DataElement::MatrixA:
            if (_A != NULL){delete _A;}
            _A = m->DeepCopy(); break;
        case Solver::DataElement::Matrixb:
            if (_b != NULL){delete _b;}
            _b = m->DeepCopy(); break;
        case Solver::DataElement::MatrixL:
            if (_L != NULL){delete _L;}
            _L = m->DeepCopy(); break;
        case Solver::DataElement::MatrixU:
            if (_U != NULL){delete _U;}
            _U = m->DeepCopy(); break;
        default:
            throw invalid_argument(_INVALID_DATA_ELEMENT_ADD_MSG);
    }
}

//See citation note 1
void LuSolver::CalculateColumnForL(const uint column, const uint limit){
    METHODHEADER_LU;
    for (uint row = column; row < limit; row++) {
        double sum = 0.0;

        for (uint iterIndex = 0; iterIndex < column; iterIndex++)
            sum += (*_L)(row,iterIndex) * (*_U)(iterIndex,column);

        (*_L)(row,column) = (*_A)(row,column) - sum;
    }
}

//See citation note 1
void LuSolver::CalculateRowForU(const uint row, const uint limit){
    METHODHEADER_LU;
    for (uint column = row; column < limit; column++) {
			double sum = 0.0;

			for(uint iterIndex = 0; iterIndex < row; iterIndex++) {
				sum += (*_L)(row,iterIndex) * (*_U)(iterIndex,column);
			}
            //Need devide by zero check?


			(*_U)(row,column) = ((*_A)(row,column) - sum) / (*_L)(row,row);
		}
}

void LuSolver::CalculateBackSolveVector(){
    METHODHEADER_LU;
    GaussianSolver gs;
    Matrix* aug = Matrix::CreateAugmentedMatrix(_U, _y);

    gs.AddDataElement(GaussianSolver::DataElement::MatrixA, aug);
    gs.AddDataElement(GaussianSolver::DataElement::Matrixb, _y);
    gs.SolveCompletely();

    delete _result;
    _result = gs.GetDataElement(GaussianSolver::DataElement::Result);
}

void LuSolver::CalculateForwardSolveVector(){
    METHODHEADER_LU;
    //Given the equation Ly=b, find vector y

    if (_L == NULL || !_L->IsInCroutLForm())
        throw invalid_argument("L factor is either null or not in proper form");

    this->_y = new Matrix(_b->RowCount(), 1);

    for(uint r = 0; r < _y->RowCount(); r++)
        (*_y)(r,0) = CalculateYVectorRow(r, _y);
}

double LuSolver::CalculateYVectorRow(const uint targetIndex, const Matrix* y) const{
    METHODHEADER_LU;
    double result = (*_b)(targetIndex,0);
    uint ir = 0, ic = 0;

    while(ir < targetIndex && ic < targetIndex)
        result -= (*_L)(targetIndex, ic++) * (*y)(ir++, 0);

    result /= (*_L)(targetIndex, targetIndex);

    return result;
}

void LuSolver::Clear(){
    METHODHEADER_LU;
    delete _A;
    delete _b;
    delete _L;
    delete _U;
    delete _y;
    delete _result;
}

//See citation note 1
void LuSolver::CroutFactoringAlgorithm() {
    METHODHEADER_LU;

    if (_A == NULL)
        throw invalid_argument("Missing data element: Matrix A.");

    if (!_A->IsSquareMatrix())
        throw invalid_argument("Matrix A is not square.");

    InitializeCroutU();
    InitializeCroutL();

	for (uint index = 0; index < _A->ColumnCount(); index++) {
		CalculateColumnForL(index, _A->ColumnCount());
		CalculateRowForU(index, _A->RowCount());
	}
}

Matrix* LuSolver::GetDataElement(Solver::DataElement param){
    METHODHEADER_LU;
    Matrix* result = NULL;
    switch(param){
        case Solver::DataElement::MatrixA:
            return _A->DeepCopy();
        case Solver::DataElement::Matrixb:
            return _b->DeepCopy();
        case Solver::DataElement::MatrixL:
            return _L->DeepCopy();
        case Solver::DataElement::MatrixU:
            return _U->DeepCopy();
        case Solver::DataElement::Matrixy:
            return _y->DeepCopy();
        case Solver::DataElement::Result:
            return _result->DeepCopy();
        default:
            throw invalid_argument(_INVALID_DATA_ELEMENT_GET_MSG);
    }
    return result;
}

void LuSolver::InitializeCroutL(){
    METHODHEADER_LU;
    delete _L;
    _L = Matrix::CreateIdentity(_A->RowCount());
}

//See citation note 1
void LuSolver::InitializeCroutU(){
    METHODHEADER_LU;
    delete _U;
    _U = Matrix::CreateIdentity(_A->RowCount());
}

void LuSolver::PrintPrettyResults(ostream& out)const{
    METHODHEADER_LU;
    out << "\nLU Solver Results:"
        << "\nMatrix A:" << endl;
    _A->PrintPretty(out);
    out << "\nMatrix b:" << endl;
    _b->PrintPretty(out);
    out << "\nResult Matrix:" << endl;
    _result->PrintPretty(out);
    out << "\nTwo norm of error: "
        << MatrixOperations::TwoNormOfError(_A,_b,_result) << endl;
}

bool LuSolver::SolveCompletely(){
    METHODHEADER_LU;
    bool result = false;
    try{
        CroutFactoringAlgorithm();
        CalculateForwardSolveVector();
        CalculateBackSolveVector();
        result = true;
    }catch (exception ex){
        SetFailMessage(ex);
    }
    return result;
}
