#include <fstream>
#include <iostream>

#include "lib/Matrix.hxx"
#include "lib/Solver.hxx"
#include "lib/Utilities.hxx"

#include "finalcommon.cpp"

using namespace std;

int main(){
    JacobiSolver js;
    Matrix *a, *b, *result;
    ifstream file("input/q6data.txt");

    try{
        uint r,c;
        file >> r >> c;

        a = new Matrix(r,c);
        b = new Matrix(r, 1);

        ReadMatFromFile(a, file);
        ReadMatFromFile(b, file);

        js.AddDataElement(JacobiSolver::DataElement::MatrixA, a);
        js.AddDataElement(JacobiSolver::DataElement::Matrixb, b);

        if (js.SolveCompletely()){
            result = js.GetDataElement(JacobiSolver::DataElement::Result);
            cout << "Two norm: " << MatrixOperations::TwoNormOfError(a,b,result) << endl;
            cout << "Number of iterations: " << js.GetIterationCount() << endl;

            PrintResult(result, "output/solution.txt");
        }else
            cout << "Solver failed! Cause: " << js.GetFailMessage() << endl;

        delete a;
        delete b;
        delete result;

    }catch(exception ex){
        cerr << "Unhandled exception: " << ex.what() << endl;
    }

    file.close();
    return 0;
}
