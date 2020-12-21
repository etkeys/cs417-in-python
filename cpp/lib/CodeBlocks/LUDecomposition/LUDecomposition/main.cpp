#include <fstream>
#include <iostream>

#include "Solver.hxx"
#include "Matrix.hxx"
#include "Utilities.hxx"

using namespace std;

const string F_T_A = "files/t4.A.40_40.def";
const string F_T_b = "files/t4.b.40_1.def";

void CreateFromFile(Matrix *&m, string fileSpec){
    Tracer _t_(__FUNCTION__,true);
    ifstream file(fileSpec);

    m = Matrix::CreateFromInputStream(file, Matrix::StreamType::BasicText);

    file.close();
}

void Test1(){
    Tracer _t_(__FUNCTION__,true);
    LuSolver lus;
    Matrix *A, *b;

    CreateFromFile(A, F_T_A);
    CreateFromFile(b, F_T_b);

    lus.AddDataElement(LuSolver::DataElement::MatrixA, A);
    lus.AddDataElement(LuSolver::DataElement::Matrixb, b);

    lus.CroutFactoringAlgorithm();
    cout << "\nMatrixL: " << endl;
        lus.GetDataElement(LuSolver::DataElement::MatrixL)->PrintPretty(cout);
    cout << "Matrix is in L form? " <<
        lus.GetDataElement(LuSolver::DataElement::MatrixL)->IsInCroutLForm() << endl;



    cout << "\nMatrixU: " << endl;
        lus.GetDataElement(LuSolver::DataElement::MatrixU)->PrintPretty(cout);
    cout << "Matrix is in upper row echilon? " <<
        lus.GetDataElement(LuSolver::DataElement::MatrixU)->IsInUpperRowEchilonForm() << endl;

    delete A;
    delete b;
}

void Test2(){
    Tracer _t_(__FUNCTION__,true);
    LuSolver lus;
    Matrix *A, *b;

    CreateFromFile(A, F_T_A);
    CreateFromFile(b, F_T_b);

    lus.AddDataElement(LuSolver::DataElement::MatrixA, A);
    lus.AddDataElement(LuSolver::DataElement::Matrixb, b);

    lus.CroutFactoringAlgorithm();
    lus.CalculateForwardSolveVector();

    cout << "\nMatrixL: " << endl;
        lus.GetDataElement(LuSolver::DataElement::MatrixL)->PrintPretty(cout);
    cout << "\nMatrixU: " << endl;
        lus.GetDataElement(LuSolver::DataElement::MatrixU)->PrintPretty(cout);

    cout << "\nMatrixy: " << endl;
        lus.GetDataElement(LuSolver::DataElement::Matrixy)->PrintPretty(cout);


    delete A;
    delete b;
}

void Test3(){
    Tracer _t_(__FUNCTION__,true);
    LuSolver lus;
    Matrix *A, *b;

    CreateFromFile(A, F_T_A);
    CreateFromFile(b, F_T_b);

    lus.AddDataElement(LuSolver::DataElement::MatrixA, A);
    lus.AddDataElement(LuSolver::DataElement::Matrixb, b);

    lus.CroutFactoringAlgorithm();
    lus.CalculateForwardSolveVector();
    lus.CalculateBackSolveVector();

    cout << "\nMatrixL: " << endl;
        lus.GetDataElement(LuSolver::DataElement::MatrixL)->PrintPretty(cout);
    cout << "\nMatrixU: " << endl;
        lus.GetDataElement(LuSolver::DataElement::MatrixU)->PrintPretty(cout);
    cout << "\nMatrixy: " << endl;
        lus.GetDataElement(LuSolver::DataElement::Matrixy)->PrintPretty(cout);

    lus.PrintPrettyResults(cout);

    delete A;
    delete b;
}

void Test4(){
    Tracer _t_(__FUNCTION__,true);
    LuSolver lus;
    Matrix *A, *b;

    CreateFromFile(A, F_T_A);
    CreateFromFile(b, F_T_b);

    lus.AddDataElement(LuSolver::DataElement::MatrixA, A);
    lus.AddDataElement(LuSolver::DataElement::Matrixb, b);

    if (lus.SolveCompletely()){
        /*
        cout << "\nResult: " << endl;
            lus.GetDataElement(LuSolver::DataElement::Result)->PrintPretty(cout);
        cout << "Two norm of the error: " << MatrixOperations::TwoNormOfError(A, b,
                                                lus.GetDataElement(LuSolver::DataElement::Result))
                                            << endl;
        */
        lus.PrintPrettyResults(cout);
    }else
        cout << "\nSolve failed.\n" << lus.GetFailMessage();

    delete A;
    delete b;
}

void Test5(){
    Tracer _t_(__FUNCTION__,true);
    GaussianSolver gs;
    Matrix *A, *b, *aug, *result;

    CreateFromFile(A, F_T_A);
    CreateFromFile(b, F_T_b);

    aug = Matrix::CreateAugmentedMatrix(A, b);

    gs.AddDataElement(GaussianSolver::DataElement::MatrixA, aug);
    gs.AddDataElement(GaussianSolver::DataElement::Matrixb, b);

    if (gs.SolveCompletely()){
        result = gs.GetDataElement(GaussianSolver::DataElement::Result);
        cout << "Two Norm of Error: " << MatrixOperations::TwoNormOfError(A, b, result) << endl;
    }else
        cerr << "Error in GaussianSolver:\n" << gs.GetFailMessage() << endl;
}

int main(){
    Tracer _t_(__FUNCTION__,true);

    Test1();
    Test2();
    Test3();
    Test4();
    Test5();

    return 0;
}
