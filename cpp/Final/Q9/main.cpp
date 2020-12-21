#include <cmath>
#include <fstream>
#include <iostream>

#include "lib/Solver.hxx"
#include "lib/Utilities.hxx"

using namespace std;

double FOfX(double x){
    return pow(x,2) - sin(x);
}

double FPrimeOfX(double x){
    return (2*x) - cos(x);
}

int main(){
    NewtonSolver ns;

    ns.AddDelegateElement(NewtonSolver::DelegateElement::FofX, FOfX);
    ns.AddDelegateElement(NewtonSolver::DelegateElement::FPrimeofX,FPrimeOfX);

    if (ns.SolveCompletely()){
        ofstream oFile("output/solution.txt");
        ns.PrintPrettyResults(oFile);
        oFile.close();

    }else
        cout << "Solver failed! Cause:\n" << ns.GetFailMessage() << endl;

    return 0;
}
