#include <fstream>
#include <iostream>

#include "Matrix.hxx"
#include "Utilities.hxx"

using namespace std;

void CreateFromFile(Matrix *&m, string fileSpec){
    Tracer _t_(__PRETTY_FUNCTION__, true);
    ifstream file(fileSpec);

    m = Matrix::CreateFromInputStream(file, Matrix::StreamType::BasicText);

    file.close();
}

void CreateFromFile(string fileSpec){
    Tracer _t_(__PRETTY_FUNCTION__, true);
    PrintDebug(cerr, "Creating from file: " + fileSpec);
    ifstream file(fileSpec);
    Matrix* m = Matrix::CreateFromInputStream(file, Matrix::StreamType::BasicText);

    cout << "\nMatrix: " << endl; m->PrintPretty(cout);

    file.close();
    delete m;
}

void CreateFromRedirectedInput(){
    Tracer _t_(__PRETTY_FUNCTION__, true);
}

void Test1(){
    Tracer _t_(__PRETTY_FUNCTION__,true);
    CreateFromFile("sampleInputs/t1.1.dat");
    CreateFromFile("sampleInputs/t1.2.dat");
}

void Test2(){
    Tracer _t_(__PRETTY_FUNCTION__,true);
    Matrix* mat1;

    mat1 = Matrix::CreateIdentity(4);
    PrintDebug(cerr, "Created matrix1");
    cout << "\nMatrix 1:" << endl; mat1->PrintPretty(cout);
    cout << "Matrix 1 is identity? " << mat1->IsIdentity() << endl;
    cout << "Matrix 1 is within bounds(3,5)? " << mat1->IsWithinBounds(3,5) << endl;
    cout << "Matrix 1 is within bounds(3,3)? " << mat1->IsWithinBounds(3,3) << endl;
    cout << "Matrix 1 is within bounds(6,3)? " << mat1->IsWithinBounds(6,3) << endl;
    cout << "Matrix 1 is within bounds(1,2)? " << mat1->IsWithinBounds(1,2) << endl;
    cout << "Matrix 1 is signular matrix? " << mat1->IsSingularMatrix() << endl;
    (*mat1)(1,1) = 0.0;
    cout << "Matrix 1 is signular matrix? " << mat1->IsSingularMatrix() << endl;

    PrintDebug(cerr, "Swapping rows in matrix1");
    mat1->SwapRows(1,2);
    PrintDebug(cerr, "Done swapping rows");
    cout << "\nMatrix 1: " << endl; mat1->PrintPretty(cout);

    delete mat1;
}

void Test3(){
    Tracer _t_(__PRETTY_FUNCTION__,true);
    Matrix* mat2;

    mat2 = Matrix::CreateRandom(3,1);
    PrintDebug(cerr, "Created matrix2");
    cout << "\nMatrix 2:" << endl; mat2->PrintPretty(cout);
    cout << "Matrix 2 is square? " << mat2->IsSquareMatrix() << endl;
    cout << "\nMatrix 2 using <<: \n" << *mat2 << endl;

    delete mat2;
}

void Test4(){
    Tracer _t_(__PRETTY_FUNCTION__,true);
    Matrix* mat3;

    mat3 = Matrix::CreateRandomDiagonalDominate(3,3);
    PrintDebug(cerr,"Created matrix3");
    cout << "\nMatrix 3:" << endl; mat3->PrintPretty(cout);
    cout << "Matrix 3 is square? " << mat3->IsSquareMatrix() << endl;
    cout << "\nMatrix 3 using <<: \n" << *mat3 << endl;

    delete mat3;
}

void Test5(){
    Tracer _t_(__PRETTY_FUNCTION__,true);
    Matrix *mat2, *mat3, *mat4;

    mat2 = Matrix::CreateRandom(3,1);
    mat3 = Matrix::CreateRandomDiagonalDominate(3,3);
    mat4 = Matrix::CreateAugmentedMatrix(mat3, mat2);
    PrintDebug(cerr,"Created matrix4");
    cout << "\nMatrix 4:" << endl; mat4->PrintPretty(cout);
    cout << "Matrix 4 is Augmented? " << mat4->IsAugmented() << endl;

    delete mat2;
    delete mat3;
    delete mat4;
}

void Test6(){
    Tracer _t_(__PRETTY_FUNCTION__,true);
    Matrix *mat1, *mat4;

    mat4 = Matrix::CreateRandom(5,5);
    PrintDebug(cerr, "Copying matrix4 to matrix1");
    mat1 = mat4->DeepCopy();
    delete mat4;
    PrintDebug(cerr, "Created copy from matrix4");
    cout << "\nMatrix 1: " << endl; mat1->PrintPretty(cout);

    delete mat1;
}

void Test7(){
    Tracer _t_(__PRETTY_FUNCTION__,true);
    Matrix *m1, *m2, *result;

    CreateFromFile(m1, "sampleInputs/t7.1.dat");
    CreateFromFile(m2, "sampleInputs/t7.2.dat");

    result = MatrixOperations::Multiply(m1, m2);
    cout << "\nResult matrix: " << endl; result->PrintPretty(cout);

    delete m1;
    delete m2;
    delete result;
}

void Test8(){
    Tracer _t_(__PRETTY_FUNCTION__,true);;
    Matrix *m1, *m2, *result;

    CreateFromFile(m1, "sampleInputs/t7.1.dat");
    CreateFromFile(m2, "sampleInputs/t7.2.dat");

    result = MatrixOperations::Add(m1, m2);
    cout << "\nResult matrix: " << endl; result->PrintPretty(cout);

    delete m1;
    delete m2;
    delete result;
}

void Test9(){
    Tracer _t_(__PRETTY_FUNCTION__,true);
    Matrix *m1, *m2, *result;

    m1 = Matrix::CreateRandomDiagonalDominate(5,5);
    m2 = Matrix::CreateRandom(5,1);

    result = MatrixOperations::Multiply(m1, m2);
    cout << "\nResult matrix: " << endl; result->PrintPretty(cout);

    delete m1;
    delete m2;
    delete result;
}

void Test10(){
    Tracer _t_(__PRETTY_FUNCTION__,true);
    Matrix *m1, *m2, *aug;
    uint firstOfB;

    CreateFromFile(m1, "sampleInputs/t1.4_4.dat");
    CreateFromFile(m2, "sampleInputs/t1.4_1.dat");

    cout << "\nMatrix 1: " << endl; m1->PrintPretty(cout);
    cout << "Matrix 1 is Augmented? " << m1->IsAugmented() << endl;
    cout << "\nMatrix 2: " << endl; m2->PrintPretty(cout);
    cout << "Matrix 2 is Augmented? " << m2->IsAugmented() << endl;
    firstOfB = m1->ColumnCount();

    aug = Matrix::CreateAugmentedMatrix(m1, m2);
    cout << "\nMatrix 1: " << endl; aug->PrintPretty(cout);
    cout << "Augmented is Augmented? " << aug->IsAugmented() << endl;
    delete m1;
    delete m2;

    //Flip m1 and m2 so we can tell that we did the extraction correctly
    m2 = Matrix::CreateAFromAugmented(aug, firstOfB);
    m1 = Matrix::CreateBFromAugmented(aug, firstOfB);

    cout << "\nMatrix 1: " << endl; m2->PrintPretty(cout);
    cout << "Matrix 1 is Augmented? " << m1->IsAugmented() << endl;
    cout << "\nMatrix 2: " << endl; m1->PrintPretty(cout);
    cout << "Matrix 2 is Augmented? " << m2->IsAugmented() << endl;

    delete m1;
    delete m2;
    delete aug;
}

void Test11(){
    Tracer _t_(__PRETTY_FUNCTION__,true);;
    Matrix *m1, *m2, *result;

    CreateFromFile(m1, "sampleInputs/t7.1.dat");
    CreateFromFile(m2, "sampleInputs/t7.2.dat");

    result = MatrixOperations::Subtract(m1, m2);
    cout << "\nResult matrix: " << endl; result->PrintPretty(cout);

    delete m1;
    delete m2;
    delete result;
}

void Test12(){
    Tracer _t_(__PRETTY_FUNCTION__,true);
    Matrix* m;

    CreateFromFile(m, "sampleInputs/t12.3_3.dat");

    cout << "\nMatrix before inversion: " << endl; m->PrintPretty(cout);
    m->Invert();
    cout << "\nMatrix after inversion: " << endl; m->PrintPretty(cout);

    delete m;
}

int main(){
    Tracer _t_(__PRETTY_FUNCTION__,true);
/*
    Test1();
    Test2();
    Test3();
    Test4();
    Test5();
    Test6();
    Test7();
    Test8();
    Test9();
    Test10();
    Test11();
*/
    Test12();

    return 0;
}
