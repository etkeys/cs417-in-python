#include <cmath>
#include <iostream>

#include "lib/Matrix.hxx"
#include "lib/Solver.hxx"
#include "lib/Utilities.hxx"

#include "finalcommon.cpp"

#ifndef METHODHEADER_MAIN
    #define METHODHEAER_MAIN Tracer _t_(__FUNCTION__,true)
#endif // METHODHEADER_MAIN

using namespace std;

int main(){
    LuSolver ls;
    Matrix *a, *b, *result;
    ifstream ifile("input/q2data.txt");

    try{
        uint r,c;
        ifile >> r >> c;

        a = new Matrix(r,c);
        b = new Matrix(r,1);

        ReadMatFromFile(a,ifile);
        ReadMatFromFile(b,ifile);

#if DEBUG
        ofstream outA("output/MatA.txt"), outB("output/MatB.txt");

        outA << (*a);
        outB << (*b);

        outA.close(); outB.close();
#endif // DEBUG

        ls.AddDataElement(LuSolver::DataElement::MatrixA, a);
        ls.AddDataElement(LuSolver::DataElement::Matrixb, b);

        if (ls.SolveCompletely()){
            result = ls.GetDataElement(LuSolver::DataElement::Result);
            cout << "Two norm: " << MatrixOperations::TwoNormOfError(a, b, result);

            PrintResult(result);
        }else
            cout << "Solver failed! Cause:\n" << ls.GetFailMessage() << endl;

        delete a;
        delete b;
        delete result;

    }catch(exception ex){
        cerr << '\n' << ex.what();
    }

    ifile.close();
    return 0;
}
