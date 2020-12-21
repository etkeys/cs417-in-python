#include <fstream>
#include <iostream>
#include <string>

#include "lib/Matrix.hxx"
#include "lib/Utilities.hxx"

using namespace std;

void WriteToFile(Matrix *m, string fileName){
    ofstream file("files/" + fileName);

    file << (*m);

    file.close();
}

int main(){
    int matSize;

    cout << "Enter the number of rows in Matrix A: ";
    cin >> matSize;
    cout << endl;

    Matrix *A, *b, *soln;

    PrintDebug(cerr, "Creating A");
    A = Matrix::CreateRandomDiagonalDominate(matSize, matSize);

    PrintDebug(cerr, "Creating soln");
    soln = Matrix::CreateRandom(matSize, 1);

    PrintDebug(cerr, "Multiplying A*soln");
    b = MatrixOperations::Multiply(A, soln);

    PrintDebug(cerr, "Writing A");
    WriteToFile(A, "A.def");

    PrintDebug(cerr, "Writing b");
    WriteToFile(b, "b.def");

    PrintDebug(cerr, "Writing soln");
    WriteToFile(soln, "soln.def");

    cout << "Done";

    return 0;
}
