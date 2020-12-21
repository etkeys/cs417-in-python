#include <fstream>
#include <iostream>
#include <string>

#include "GaussianSolver.hpp"
#include "../../Matrix/Matrix/Matrix.hpp"

using namespace std;

void CreateFromFile(Matrix *&m, string fileSpec){
    ifstream file(fileSpec);

    m = Matrix::CreateFromInputStream(file, Matrix::StreamType::BasicText);

    file.close();
}

int main(){
    GaussianSolver gs;
    Matrix *m1, *m2;

    CreateFromFile(m1, "sampleInputs/t1.4_4.dat");
    CreateFromFile(m2, "sampleInputs/t1.4_1.dat");

    cout << "\nMatrix 1: " << endl; m1->PrintPretty(cout);
    cout << "\nMatrix 2: " << endl; m2->PrintPretty(cout);

    m1 = Matrix::CreateAugmentedMatrix(m1,m2);
    cout << "\nMatrix 1: " << endl; m1->PrintPretty(cout);

    gs.AddInput(m1);
    gs.ConvertInputToUpperRowEchilon();
    cout << "\nGS after upper row echilon: " << endl;
    gs.GetResult()->PrintPretty(cout);

    return 0;
}
