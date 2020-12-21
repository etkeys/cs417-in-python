#include <cmath>
#include <fstream>
#include <iostream>

#include "lib/Solver.hxx"
#include "lib/SteepestDescentSolver.hxx"
#include "lib/Utilities.hxx"

using namespace std;

double HSolver(double x, double y, double a, double b){
    double result = 0.0;

    result = (-1)*(50*x*a) - (40*y*b) + (2*a) + (b);
    result /= ((50*pow(a,2)) + (40*pow(b,2)));
    return result;
}

double HPrimeX(double x, double y){
    double result = 0.0;

    result = (50*x) - 2;
    return result;
}

double HPrimeY(double x, double y){
    double result = 0.0;

    result = (40*y) - 1;
    return result;
}

int main(){
    SteepestDescentSolver sds;
    double guessx = -3, guessy = 3;

    sds.SetHPrimeXDelegate(HPrimeX);
    sds.SetHPrimeYDelegate(HPrimeY);
    sds.SetHSolverDelegate(HSolver);

    sds.SetGuessX(guessx);
    sds.SetGuessY(guessy);

    sds.SetDiffTolerance(0.001);

    if (sds.SolveCompletely()){
        ofstream oFile("output/solution.txt");
        sds.PrintPrettyResults(oFile);
        oFile.close();
    }else{
        cout << "Solver failed! Cause:\n" << sds.GetFailMessage() << endl;
    }

    return 0;
}
