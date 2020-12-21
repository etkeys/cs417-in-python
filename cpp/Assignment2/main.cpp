#include <fstream>
#include <iostream>
#include <string>

#include "lib/Matrix.hxx"
#include "lib/Solver.hxx"
#include "lib/Utilities.hxx"

#ifndef METHODHEADER_MAIN
    #define METHODHEADER_MAIN Tracer _t_(__FUNCTION__,true);
#endif

using namespace std;

void CreateFromFile(Matrix*& m, string filespec){
    METHODHEADER_MAIN;
    ifstream file(filespec);

    m = Matrix::CreateFromInputStream(file,Matrix::StreamType::BasicText);

    file.close();
}

int main(){
    METHODHEADER_MAIN;
    LuSolver lus;
    Matrix *A, *b;

    CreateFromFile(A, "files/A.def");
    CreateFromFile(b, "files/b.def");

    lus.AddDataElement(LuSolver::DataElement::MatrixA, A);
    lus.AddDataElement(LuSolver::DataElement::Matrixb, b);

    if (lus.SolveCompletely()){
        lus.PrintPrettyResults(cout);
    }else
        cout << "A problem occured during processing. Cause:\n" << lus.GetFailMessage() << endl;

    delete A;
    delete b;

    return 0;
}
