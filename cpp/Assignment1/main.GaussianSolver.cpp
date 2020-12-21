#include <fstream>
#include <iostream>
#include <string>

#include "lib/GaussianSolver.hxx"
#include "lib/Matrix.hxx"
#include "lib/Utilities.hxx"

using namespace std;

void CreateFromFile(Matrix *&m, string fileSpec){
    Tracer _t_(__FUNCTION__, true);
    ifstream file("files/" + fileSpec);

    m = Matrix::CreateFromInputStream(file, Matrix::StreamType::BasicText);

    file.close();
}

using namespace std;

int main(){
    Tracer _t_(__FUNCTION__, true);
    GaussianSolver gs;
    Matrix *A, *b, *aug, *result;

    CreateFromFile(A, "A.def");
    CreateFromFile(b, "b.def");
    aug = Matrix::CreateAugmentedMatrix(A,b);

    gs.AddDataElement(GaussianSolver::DataElement::MatrixA, aug);

    if (gs.SolveCompletely()){
        result = gs.GetDataElement(GaussianSolver::DataElement::Result);
        cout << "\nGaussian solver succeeded. Result matrix:" << endl;
        result->PrintPretty(cout);
        cout << "\nTwo norm of the error: " << MatrixOperations::TwoNormOfError(A, b, result) << endl;

    }else{
        cout << "\nGaussian solver failed. Fail message:"
                << "\n" << gs.GetFailMessage() << endl;
    }

    delete A;
    delete b;
    delete aug;
    delete result;

    return 0;
}
