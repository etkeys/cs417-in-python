#include <fstream>
#include <iostream>
#include <string>

#include "lib/Matrix.hxx"
#include "lib/Solver.hxx"
#include "lib/Utilities.hxx"


using namespace std;

const string INPUTFILEA = "files/A.def";
const string INPUTFILEB = "files/b.def";
const string INPUTFILEOMEGA = "files/omega.def";

void CheckForLameConvergence(JacobiSolver* s){
    Tracer _t_(__FUNCTION__, true);
    if (s->HasLameConvergence())
        cout << "SOLVER DETECTED LAME CONVERGENCE!!" << endl;
}

void CreateFromFile(Matrix*& m, string filespec){
    Tracer _t_(__FUNCTION__, true);
    ifstream file(filespec);

    m = Matrix::CreateFromInputStream(file, Matrix::StreamType::BasicText);

    file.close();
}

void SolveUsingGausSeidel(){
    Tracer _t_(__FUNCTION__,true);
    GausSeidelSolver gss;
    Matrix *A, *b;

    cout << "\nSolving using Gaus-Seidel method."
        << "\nThis method solve problems in the form of:"
        << "\n\tx^(k+1) = Dx^(k)+C ; where:"
        << "\n\tD = -(L^-1)U,"
        << "\n\tC = (L^-1)b,"
        << "\n\tL = Lower triangular component of A,"
        << "\n\tU = Strict upper triangular compnent of A." << endl;

    CreateFromFile(A, INPUTFILEA);
    CreateFromFile(b, INPUTFILEB);

    gss.AddDataElement(GausSeidelSolver::DataElement::MatrixA, A);
    gss.AddDataElement(GausSeidelSolver::DataElement::Matrixb, b);

    if (gss.SolveCompletely()){
        /*
        result = gss.GetDataElement(GausSeidelSolver::DataElement::Result);
        double twoNorm = MatrixOperations::TwoNormOfError(A,b,result);

        CheckForLameConvergence(&gss);
        cout << "\nSolver completed! Result matrix:" << endl;
        result->PrintPretty(cout);
        cout << "Number of iterations: " << gss.GetIterationCount() << endl;
        cout << "Two norm of the error: " << twoNorm << endl;
        */
        gss.PrintPrettyResults(cout);

    }else
        cout << "\nSolver failed! Cause:\n"
            << gss.GetFailMessage() << endl;

    //delete A;
    //delete b;
    //delete result;
}

void SolveUsingJacobi(){
    Tracer _t_(__FUNCTION__,true);
    JacobiSolver js;
    Matrix *A, *b;

    cout << "\nSolving using Jacobi method."
        << "\nThis method solve problems in the form of:"
        <<  "\n\tx^(k+1) = (D^-1)(b-Cx^k)" << endl;

    CreateFromFile(A, INPUTFILEA);
    CreateFromFile(b, INPUTFILEB);

    js.AddDataElement(JacobiSolver::DataElement::MatrixA, A);
    js.AddDataElement(JacobiSolver::DataElement::Matrixb, b);

    if (js.SolveCompletely()){
        /*
        result = js.GetDataElement(JacobiSolver::DataElement::Result);
        double twoNorm = MatrixOperations::TwoNormOfError(A,b,result);

        CheckForLameConvergence(&js);
        cout << "\nSolver completed! Result matrix:" << endl;
        result->PrintPretty(cout);
        cout << "Number of iterations: " << js.GetIterationCount() << endl;
        cout << "Two norm of the error: " << twoNorm << endl;
        */
        js.PrintPrettyResults(cout);

    }else
        cout << "\nSolver failed! Cause:\n"
            << js.GetFailMessage() << endl;

    //delete A;
    //delete b;
    //delete result;
}

void SolveUsingSor(){
    Tracer _t_(__FUNCTION__,true);
    SorSolver ss;
    Matrix *A, *b, *omega;

    cout << "\nSolving using SOR method."
        << "\nThis method solve problems in the form of:"
        << "\n\tx^(k+1) =(1-w)x^(k) + w(Dx^(k)+C) ; where:"
        << "\n\tD = -(L^-1)U,"
        << "\n\tC = (L^-1)b,"
        << "\n\tL = Lower triangular component of A,"
        << "\n\tU = Strict upper triangular compnent of A."
        << "\n\tw = Relaxation factor; 0 < w < 2." << endl;

    CreateFromFile(A, INPUTFILEA);
    CreateFromFile(b, INPUTFILEB);
    CreateFromFile(omega, INPUTFILEOMEGA); //Stored in a 1x1 matrix

    ss.AddDataElement(SorSolver::DataElement::MatrixA, A);
    ss.AddDataElement(SorSolver::DataElement::Matrixb, b);
    ss.AddDataElement(SorSolver::DataElement::SorOmega, omega);

    if (ss.SolveCompletely()){
        /*
        result = ss.GetDataElement(SorSolver::DataElement::Result);
        double twoNorm = MatrixOperations::TwoNormOfError(A,b,result);

        CheckForLameConvergence(&ss);
        cout << "\nSolver completed! Result matrix:" << endl;
        result->PrintPretty(cout);
        cout << "\nOmega used:" << endl;
        omega->PrintPretty(cout);
        cout << "Number of iterations: " << ss.GetIterationCount() << endl;
        cout << "Two norm of the error: " << twoNorm << endl;
        */
        ss.PrintPrettyResults(cout);

    }else
        cout << "\nSolver failed! Cause:\n"
            << ss.GetFailMessage() << endl;

    //delete A;
    //delete b;
    //delete omega;
    //delete result;
}

int main(){
    Tracer _t_(__FUNCTION__,true);

    SolveUsingJacobi();
    SolveUsingGausSeidel();
    SolveUsingSor();

    return 0;
}
