#include <fstream>
#include <iostream>
#include <string>

#include "Matrix.hxx"
#include "Solver.hxx"
#include "Utilities.hxx"

using namespace std;

const string TESTFILEA = "files/T2.A.20_20.def";
const string TESTFILEB = "files/T2.b.20_1.def";
const string TESTFILEOMEGA = "files/T2.Omega.def";

void CheckForLameConvergence(JacobiSolver* js){
    if (js->HasLameConvergence())
        cout << "SOLVER DETECTED LAME CONVERGENCE!!" << endl;
}

void CreateFromFile(Matrix*& m, string filespec){
    Tracer _t_(__PRETTY_FUNCTION__, true);
    ifstream file(filespec);

    m = Matrix::CreateFromInputStream(file, Matrix::StreamType::BasicText);

    file.close();
}

void JacobiTest1(){
    Tracer _t_(__FUNCTION__,true);
    JacobiSolver js;
    Matrix *_A, *_b;

    CreateFromFile(_A, TESTFILEA);
    CreateFromFile(_b, TESTFILEB);

    js.AddDataElement(JacobiSolver::DataElement::MatrixA, _A);
    js.AddDataElement(JacobiSolver::DataElement::Matrixb, _b);

    js.CreateInterimMatrices();

    cout << "\nMatrix A: " << endl;
    js.GetDataElement(JacobiSolver::DataElement::MatrixA)->PrintPretty(cout);

    cout << "\nMatrix b: " << endl;
    js.GetDataElement(JacobiSolver::DataElement::Matrixb)->PrintPretty(cout);

    cout << "\nMatrix C: " << endl;
    js.GetDataElement(JacobiSolver::DataElement::MatrixC)->PrintPretty(cout);
    cout << "Is diagonal term matrix? " << js.GetDataElement(
                                            JacobiSolver::DataElement::MatrixC)->IsDiagonalTermMatrix()
                                        << endl;
    cout << "Is matrix inverted? " << js.GetDataElement(
                                        JacobiSolver::DataElement::MatrixC)->IsInverted()
                                    << endl;

    cout << "\nMatrix D: " << endl;
    js.GetDataElement(JacobiSolver::DataElement::MatrixD)->PrintPretty(cout);
    cout << "Is diagonal term matrix? " << js.GetDataElement(
                                            JacobiSolver::DataElement::MatrixD)->IsDiagonalTermMatrix()
                                        << endl;
    cout << "Is matrix inverted? " << js.GetDataElement(
                                        JacobiSolver::DataElement::MatrixD)->IsInverted()
                                    << endl;

    delete _A;
    delete _b;
}

void JacobiTest2(){
    Tracer _t_(__FUNCTION__, true);
    JacobiSolver js;
    Matrix *_A, *_b;

    CreateFromFile(_A, TESTFILEA);
    CreateFromFile(_b, TESTFILEB);

    js.AddDataElement(JacobiSolver::DataElement::MatrixA, _A);
    js.AddDataElement(JacobiSolver::DataElement::Matrixb, _b);

    js.CreateInterimMatrices();
    js.IterateUntilSolved();

    cout << "\nFinal result matrix: " << endl;
    js.GetDataElement(JacobiSolver::DataElement::Result)->PrintPretty(cout);
    cout << "Number of iterations: " << js.GetIterationCount() << endl;

    cout << "Two Norm: " << MatrixOperations::TwoNormOfError(_A, _b,
                            js.GetDataElement(JacobiSolver::DataElement::Result))
                        << endl;

    CheckForLameConvergence(&js);

    delete _A;
    delete _b;
}

void JacobiTest3(){
    Tracer _t_(__FUNCTION__,true);
    JacobiSolver js;
    Matrix *_A, *_b;

    CreateFromFile(_A, TESTFILEA);
    CreateFromFile(_b, TESTFILEB);

    js.AddDataElement(JacobiSolver::DataElement::MatrixA, _A);
    js.AddDataElement(JacobiSolver::DataElement::Matrixb, _b);

    if (js.SolveCompletely()){
        /*
        cout << "Solver succeeded!!\nResult matrix: " << endl;
        js.GetDataElement(JacobiSolver::DataElement::Result)->PrintPretty(cout);
        cout << "Number of iterations: " << js.GetIterationCount() << endl;
        */

        js.PrintPrettyResults(cout);

    }else
        cout << "Solver failed!! Cause:\n" << js.GetFailMessage() << endl;

    cout << "Two Norm: " << MatrixOperations::TwoNormOfError(_A, _b,
                            js.GetDataElement(JacobiSolver::DataElement::Result))
                        << endl;

    CheckForLameConvergence(&js);

    delete _A;
    delete _b;
}

void GausSeidelTest1(){
    Tracer _t_(__FUNCTION__,true);
    GausSeidelSolver gss;
    Matrix *_A, *_b;

    CreateFromFile(_A, TESTFILEA);
    CreateFromFile(_b, TESTFILEB);

    gss.AddDataElement(GausSeidelSolver::DataElement::MatrixA, _A);
    gss.AddDataElement(GausSeidelSolver::DataElement::Matrixb, _b);

    gss.CreateInterimMatrices();

    cout << "\nMatrix A: " << endl;
    gss.GetDataElement(GausSeidelSolver::DataElement::MatrixA)->PrintPretty(cout);

    cout << "\nMatrix b: " << endl;
    gss.GetDataElement(GausSeidelSolver::DataElement::Matrixb)->PrintPretty(cout);

    cout << "\nMatrix C: " << endl;
    gss.GetDataElement(GausSeidelSolver::DataElement::MatrixC)->PrintPretty(cout);
    cout << "Is diagonal term matrix? " << gss.GetDataElement(
                                            GausSeidelSolver::DataElement::MatrixC)->IsDiagonalTermMatrix()
                                        << endl;
    cout << "Is matrix inverted? " << gss.GetDataElement(
                                        GausSeidelSolver::DataElement::MatrixC)->IsInverted()
                                    << endl;

    cout << "\nMatrix D: " << endl;
    gss.GetDataElement(GausSeidelSolver::DataElement::MatrixD)->PrintPretty(cout);
    cout << "Is diagonal term matrix? " << gss.GetDataElement(
                                            GausSeidelSolver::DataElement::MatrixD)->IsDiagonalTermMatrix()
                                        << endl;
    cout << "Is matrix inverted? " << gss.GetDataElement(
                                        GausSeidelSolver::DataElement::MatrixD)->IsInverted()
                                    << endl;

    delete _A;
    delete _b;
}

void GausSeidelTest2(){
    Tracer _t_(__FUNCTION__,true);
    GausSeidelSolver gss;
    Matrix *_A, *_b;

    CreateFromFile(_A, TESTFILEA);
    CreateFromFile(_b, TESTFILEB);

    gss.AddDataElement(GausSeidelSolver::DataElement::MatrixA, _A);
    gss.AddDataElement(GausSeidelSolver::DataElement::Matrixb, _b);

    gss.CreateInterimMatrices();
    gss.IterateUntilSolved();

    cout << "\nFinal result matrix: " << endl;
    gss.GetDataElement(GausSeidelSolver::DataElement::Result)->PrintPretty(cout);
    cout << "Number of iterations: " << gss.GetIterationCount() << endl;

    cout << "Two Norm: " << MatrixOperations::TwoNormOfError(_A, _b,
                            gss.GetDataElement(GausSeidelSolver::DataElement::Result))
                        << endl;

    CheckForLameConvergence(&gss);

    delete _A;
    delete _b;
}

void GausSeidelTest3(){
    Tracer _t_(__FUNCTION__,true);
    GausSeidelSolver gss;
    Matrix *_A, *_b;

    CreateFromFile(_A, TESTFILEA);
    CreateFromFile(_b, TESTFILEB);

    gss.AddDataElement(GausSeidelSolver::DataElement::MatrixA, _A);
    gss.AddDataElement(GausSeidelSolver::DataElement::Matrixb, _b);

    if (gss.SolveCompletely()){

        gss.PrintPrettyResults(cout);

    }else
        cout << "Solver failed!! Cause:\n" << gss.GetFailMessage() << endl;

    cout << "Two Norm: " << MatrixOperations::TwoNormOfError(_A, _b,
                            gss.GetDataElement(GausSeidelSolver::DataElement::Result))
                        << endl;

    CheckForLameConvergence(&gss);

    delete _A;
    delete _b;
}

void SorTest1(){
    Tracer _t_(__FUNCTION__,true);
    SorSolver sors;
    Matrix *A, *b, *omega;

    CreateFromFile(A, TESTFILEA);
    CreateFromFile(b, TESTFILEB);
    CreateFromFile(omega, TESTFILEOMEGA);

    sors.AddDataElement(SorSolver::DataElement::MatrixA, A);
    sors.AddDataElement(SorSolver::DataElement::Matrixb, b);
    sors.AddDataElement(SorSolver::DataElement::SorOmega, omega);

    sors.CreateInterimMatrices();

    cout << "\nMatrix A: " << endl;
    sors.GetDataElement(SorSolver::DataElement::MatrixA)->PrintPretty(cout);

    cout << "\nMatrix b: " << endl;
    sors.GetDataElement(SorSolver::DataElement::Matrixb)->PrintPretty(cout);

    cout << "\nMatrix C: " << endl;
    sors.GetDataElement(SorSolver::DataElement::MatrixC)->PrintPretty(cout);
    cout << "Is diagonal term matrix? " << sors.GetDataElement(
                                            SorSolver::DataElement::MatrixC)->IsDiagonalTermMatrix()
                                        << endl;
    cout << "Is matrix inverted? " << sors.GetDataElement(
                                        SorSolver::DataElement::MatrixC)->IsInverted()
                                    << endl;

    cout << "\nMatrix D: " << endl;
    sors.GetDataElement(SorSolver::DataElement::MatrixD)->PrintPretty(cout);
    cout << "Is diagonal term matrix? " << sors.GetDataElement(
                                            SorSolver::DataElement::MatrixD)->IsDiagonalTermMatrix()
                                        << endl;
    cout << "Is matrix inverted? " << sors.GetDataElement(
                                        SorSolver::DataElement::MatrixD)->IsInverted()
                                    << endl;

    cout << "\nOmega: " << endl;
    sors.GetDataElement(SorSolver::DataElement::SorOmega)->PrintPretty(cout);

    delete A;
    delete b;
    delete omega;
}

void SorTest2(){
    Tracer _t_(__FUNCTION__,true);
    SorSolver sors;
    Matrix *A, *b, *omega;

    CreateFromFile(A, TESTFILEA);
    CreateFromFile(b, TESTFILEB);
    CreateFromFile(omega, TESTFILEOMEGA);

    sors.AddDataElement(SorSolver::DataElement::MatrixA, A);
    sors.AddDataElement(SorSolver::DataElement::Matrixb, b);
    sors.AddDataElement(SorSolver::DataElement::SorOmega, omega);

    sors.CreateInterimMatrices();
    sors.IterateUntilSolved();

    cout << "\nFinal result matrix: " << endl;
    sors.GetDataElement(SorSolver::DataElement::SorOmega)->PrintPretty(cout);
    cout << "Number of iterations: " << sors.GetIterationCount() << endl;

    cout << "Two Norm: " << MatrixOperations::TwoNormOfError(A, b,
                            sors.GetDataElement(SorSolver::DataElement::Result))
                        << endl;

    CheckForLameConvergence(&sors);

    delete A;
    delete b;
    delete omega;
}

void SorTest3(){
    Tracer _t_(__FUNCTION__, true);

     SorSolver sors;
    Matrix *A, *b, *omega;

    CreateFromFile(A, TESTFILEA);
    CreateFromFile(b, TESTFILEB);
    CreateFromFile(omega, TESTFILEOMEGA);

    sors.AddDataElement(SorSolver::DataElement::MatrixA, A);
    sors.AddDataElement(SorSolver::DataElement::Matrixb, b);
    sors.AddDataElement(SorSolver::DataElement::SorOmega, omega);

    if (sors.SolveCompletely())
        sors.PrintPrettyResults(cout);
    else
        cout << "\nSolver failed. Cause:\n"
            << sors.GetFailMessage() << endl;

    CheckForLameConvergence(&sors);

    delete A;
    delete b;
    delete omega;
}

int main(){
    Tracer _t_(__FUNCTION__,true);

    //JacobiTest1();
    //JacobiTest2();
    JacobiTest3();
    //GausSeidelTest1();
    //GausSeidelTest2();
    GausSeidelTest3();
    //SorTest1();
    //SorTest2();
    SorTest3();

    return 0;
}
