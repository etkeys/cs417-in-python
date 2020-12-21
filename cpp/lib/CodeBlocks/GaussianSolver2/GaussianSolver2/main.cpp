#include <fstream>
#include <iostream>
#include <string>

#include "lib/Matrix.hxx"
#include "lib/Solver.hxx"
#include "lib/Utilities.hxx"

using namespace std;

void CreateFromFile(Matrix *&m, string fileSpec){
    Tracer _t_(__PRETTY_FUNCTION__,true);
    ifstream file(fileSpec);

    m = Matrix::CreateFromInputStream(file, Matrix::StreamType::BasicText);

    file.close();
}

void Makemat(){
    Tracer _t_(__PRETTY_FUNCTION__,true);
    Matrix *m1, *m2;

    m1 = Matrix::CreateRandomDiagonalDominate(5,5);
    m2 = Matrix::CreateRandom(5,1);

    m1->SwapRows(0,2);
    m1->SwapRows(1,4);

    cout << "Makemat1: " << endl; m1->PrintPretty(cout);
    cout << "Makemat2: " << endl; m2->PrintPretty(cout);

    ofstream file("sampleInputs/t3.5_5.dat");
    file << (*m1);
    file.close();

    file.open("sampleInputs/t3.5_1.dat");
    file << (*m2);
    file.close();
}

void Test1(){
    Tracer _t_(__PRETTY_FUNCTION__,true);
    GaussianSolver gs;
    Matrix *m1, *m2;

    CreateFromFile(m1, "sampleInputs/t1.4_4.dat");
    CreateFromFile(m2, "sampleInputs/t1.4_1.dat");

    cout << "\nMatrix 1: " << endl; m1->PrintPretty(cout);
    cout << "\nMatrix 2: " << endl; m2->PrintPretty(cout);

    m1 = Matrix::CreateAugmentedMatrix(m1,m2);
    cout << "\nMatrix 1: " << endl; m1->PrintPretty(cout);

    PrintDebug(cerr, "Coverting matrix to Upper Row Echilion");
    gs.AddDataElement(GaussianSolver::DataElement::MatrixA, m1);

    PrintDebug(cerr, "Creating result vector");
    gs.CalculateBackSolveVector();
    cout << "\nGS after result vector: " << endl;
    gs.GetDataElement(GaussianSolver::DataElement::Result)->PrintPretty(cout);
}

void Test2(){
    Tracer _t_(__PRETTY_FUNCTION__,true);
    GaussianSolver gs;
    Matrix *m1, *m2;

    CreateFromFile(m1, "sampleInputs/t1.4_4.dat");
    CreateFromFile(m2, "sampleInputs/t1.4_1.dat");

    m1 = Matrix::CreateAugmentedMatrix(m1,m2);

    PrintDebug(cerr, "GS::SolveCompletely");
    gs.AddDataElement(GaussianSolver::DataElement::MatrixA, m1);

    if (gs.SolveCompletely()){
        cout << "\nGS solve complete. Result: " << endl;
        gs.GetDataElement(GaussianSolver::DataElement::Result)->PrintPretty(cout);
    }else{
        cout << "\nGS solve failed. Message: "
            << gs.GetFailMessage() << endl;
    }
}

void Test3(){
    Tracer _t_(__PRETTY_FUNCTION__,true);
    GaussianSolver gs;
    Matrix *m1, *m2;

    CreateFromFile(m1, "sampleInputs/t3.5_5.dat");
    CreateFromFile(m2, "sampleInputs/t3.5_1.dat");
    cout << "\nMatrix 1: " << endl; m1->PrintPretty(cout);
    cout << "\nMatrix 2: " << endl; m2->PrintPretty(cout);

    m1 = Matrix::CreateAugmentedMatrix(m1,m2);
    cout << "\nMatrix 1: " << endl; m1->PrintPretty(cout);

    PrintDebug(cerr, "Coverting matrix to Upper Row Echilion");
    gs.AddDataElement(GaussianSolver::DataElement::MatrixA, m1);

    PrintDebug(cerr, "Creating result vector");
    gs.CalculateBackSolveVector();
    cout << "\nGS after result vector: " << endl;
    gs.GetDataElement(GaussianSolver::DataElement::Result)->PrintPretty(cout);
}

void Test4(){
    Tracer _t_(__PRETTY_FUNCTION__,true);
    GaussianSolver gs;
    Matrix *m1, *m2;

    CreateFromFile(m1, "sampleInputs/t3.5_5.dat");
    CreateFromFile(m2, "sampleInputs/t3.5_1.dat");

    cout << "\nMatrix 1: " << endl; m1->PrintPretty(cout);
    cout << "\nMatrix 2: " << endl; m2->PrintPretty(cout);

    m1 = Matrix::CreateAugmentedMatrix(m1,m2);
    cout << "\nMatrix 1: " << endl; m1->PrintPretty(cout);

    PrintDebug(cerr, "GS::SolveCompletely");
    gs.AddDataElement(GaussianSolver::DataElement::MatrixA, m1);

    if (gs.SolveCompletely()){
        cout << "\nGS solve complete. Result: " << endl;
        gs.GetDataElement(GaussianSolver::DataElement::Result)->PrintPretty(cout);
    }else{
        cout << "\nGS solve failed. Message: "
            << gs.GetFailMessage() << endl;
    }
}

void Test5(){
    Tracer _t_(__PRETTY_FUNCTION__,true);

    GaussianSolver gs;
    Matrix *a, *b, *aug;

    CreateFromFile(a, "sampleInputs/t3.5_5.dat");
    CreateFromFile(b, "sampleInputs/t3.5_1.dat");

    aug = Matrix::CreateAugmentedMatrix(a, b);
    gs.AddDataElement(GaussianSolver::DataElement::MatrixA, aug);

    if (!gs.SolveCompletely()){
        cerr << gs.GetFailMessage() << endl;
        exit(EXIT_FAILURE);
    }

    double twoNorm;
    gs.AddDataElement(GaussianSolver::DataElement::MatrixA, a);
    gs.AddDataElement(GaussianSolver::DataElement::Matrixb, b);

    twoNorm = MatrixOperations::TwoNormOfError(a, b, gs.GetDataElement(GaussianSolver::DataElement::Result));

    delete a;
    delete b;
    delete aug;

    cout << "\nTwo Norm of Error: " << twoNorm << endl;
}

int main(){
    Tracer _t_(__PRETTY_FUNCTION__,true);

    //Makemat();
    Test1();
    Test2();
    Test3();
    Test4();
    Test5();

    return 0;
}
