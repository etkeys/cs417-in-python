#include <cmath>
#include <fstream>
#include <iostream>

#include "lib/Matrix.hxx"
#include "lib/Solver.hxx"
#include "lib/Utilities.hxx"

#ifndef METHODHEADER_MAIN
    #define METHODHEAER_MAIN Tracer _t_(__FUNCTION__,true)
#endif // METHODHEADER_MAIN

using namespace std;

void PrintResult(Matrix*& result){
    ofstream file("output/solution.txt");
    uint i = sqrt((double)(result->RowCount()));

    for(uint r = 0; r < result->RowCount(); r += i){
        for(uint c = 0; c < i; c++)
            file << (*result)(r+c,0) << ' ';

        file << endl;
    }

    file.close();
}

void ReadMatFromFile(Matrix*& m, ifstream& in){
    METHODHEAER_MAIN;

    for(uint r = 0; r < m->RowCount(); r++)
        for(uint c = 0; c < m->ColumnCount(); c++)
            in >> (*m)(r,c);
}

int main(){
    GaussianSolver gs;
    Matrix *a, *b, *result;
    ifstream file("input/q1data.txt");

    try{
        uint r,c;
        file >> r >> c;

        a = new Matrix(r,c);
        b = new Matrix(r,1);

        ReadMatFromFile(a, file);
        ReadMatFromFile(b, file);

        Matrix *aug = Matrix::CreateAugmentedMatrix(a, b);

#if DEBUG
        ofstream outA("output/MatA.txt"), outB("output/MatB.txt"), outAug("output/MatAug.txt");

        outA << (*a);
        outB << (*b);
        outAug << (*aug);

        outA.close(); outB.close(); outAug.close();
#endif // DEBUG


        gs.AddDataElement(GaussianSolver::DataElement::MatrixA, aug);

        if (gs.SolveCompletely()){
            result = gs.GetDataElement(GaussianSolver::DataElement::Result);
            cout << "Two norm: " << MatrixOperations::TwoNormOfError(a, b, result) << endl;
#if DEBUG
            result->PrintPretty(cout);
            cout << "Result row count: " << result->RowCount() << endl;
#endif // DEBUG
            PrintResult(result);
        }else
            cout << "Solver failed! Cause:\n" << gs.GetFailMessage() << endl;

        delete a;
        delete b;
        delete aug;
        delete result;

    }catch(exception ex){
        cerr << '\n' << ex.what();
    }

    file.close();
    return 0;
}
