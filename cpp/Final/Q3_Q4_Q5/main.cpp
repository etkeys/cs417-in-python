#include <fstream>
#include <iostream>

#include "lib/Matrix.hxx"
#include "lib/Solver.hxx"
#include "lib/Utilities.hxx"
#include "finalcommon.cpp"

#ifndef METHODHEADER_MAIN
    #define METHODHEADER_MAIN Tracer _t_(__FUNCTION__,true)
#endif

using namespace std;

void DoQ3(){
    METHODHEADER_MAIN;
    JacobiSolver js;
    Matrix *a, *b, *result;
    ifstream file("input/q3data.txt");

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
            cout << "Two Norm: " << MatrixOperations::TwoNormOfError(a,b,result) << endl;
            cout << "Number of iterations: " << js.GetIterationCount() << endl;

            PrintResult(result, "output/solutionq3.txt");
        }else
            cout << "Solver failed! Cause: \n" << js.GetFailMessage() << endl;

    }catch(exception ex){
        cerr << "Trapped exception: " << ex.what() << endl;
    }

    file.close();
}

void DoQ4(){
    METHODHEADER_MAIN;
    JacobiSolver gss;
    Matrix *a, *b, *result;

    ifstream file("input/q4data.txt");

    try{
        uint r,c;
        file >> r >> c;

        a = new Matrix(r,c);
        b = new Matrix(r, 1);

        ReadMatFromFile(a, file);
        ReadMatFromFile(b, file);

        gss.AddDataElement(JacobiSolver::DataElement::MatrixA, a);
        gss.AddDataElement(JacobiSolver::DataElement::Matrixb, b);

        if (gss.SolveCompletely()){
            result = gss.GetDataElement(JacobiSolver::DataElement::Result);
            cout << "Two Norm: " << MatrixOperations::TwoNormOfError(a,b,result) << endl;
            cout << "Number of iterations: " << gss.GetIterationCount() << endl;

            PrintResult(result, "output/solutionq4.txt");
        }else
            cout << "Solver failed! Cause: \n" << gss.GetFailMessage() << endl;

    }catch(exception ex){
        cerr << "Trapped exception: " << ex.what() << endl;
    }

    file.close();

}
void DoQ5(){
    METHODHEADER_MAIN;
    JacobiSolver ss;
    //Matrix *a, *b, *result, *omegam;
    Matrix *a, *b, *result;
    double omegav = 1.15;

    ifstream file("input/q5data.txt");

    try{
        uint r,c;
        file >> r >> c;

        a = new Matrix(r,c);
        b = new Matrix(r, 1);
        //omegam = new Matrix(1,1);

        ReadMatFromFile(a, file);
        ReadMatFromFile(b, file);
        //(*omegam)(0,0) = omegav;

        ss.AddDataElement(JacobiSolver::DataElement::MatrixA, a);
        ss.AddDataElement(JacobiSolver::DataElement::Matrixb, b);
        //ss.AddDataElement(SorSolver::DataElement::SorOmega, omegam);

        if (ss.SolveCompletely()){
            result = ss.GetDataElement(SorSolver::DataElement::Result);
            cout << "Two Norm: " << MatrixOperations::TwoNormOfError(a,b,result) << endl;
            cout << "Number of iterations: " << ss.GetIterationCount() << endl;

            PrintResult(result, "output/solutionq5.txt");
        }else
            cout << "Solver failed! Cause: \n" << ss.GetFailMessage() << endl;

    }catch(exception ex){
        cerr << "Trapped exception: " << ex.what() << endl;
    }

    file.close();


}

int main(){

    cout << "Question3: Jacobi" << endl;DoQ3();
    cout << "Question4: Gaus-Seidel" << endl; DoQ4();
    cout << "Question5: SOR" << endl; DoQ5();

    return 0;
}
