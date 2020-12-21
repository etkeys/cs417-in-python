#include <fstream>
#include <iostream>

#include "lib/Matrix.hxx"
#include "lib/Solver.hxx"
#include "lib/Utilities.hxx"

#include "finalcommon.cpp"

using namespace std;

int main(){
    PowerSolver ps;
    Matrix *a, *result;
    ifstream file("input/q7data.txt");

    try{
        uint r,c;
        file >> r >> c;

        a = new Matrix(r,c);

        ReadMatFromFile(a, file);

        ps.AddDataElement(PowerSolver::DataElement::MatrixA, a);

        if (ps.SolveCompletely()){
            result = ps.GetDataElement(PowerSolver::DataElement::Result);
            ofstream oFile("output/solution.txt");
            ps.PrintPrettyResults(oFile);
            oFile.close();

            delete result;
        }else
            cout << "Solver failed! Cause:\n" << ps.GetFailMessage() << endl;

        delete a;

    } catch(exception ex){
        cerr << "Unhandled exception: " << ex.what() << endl;
    }

    file.close();

    return 0;
}
