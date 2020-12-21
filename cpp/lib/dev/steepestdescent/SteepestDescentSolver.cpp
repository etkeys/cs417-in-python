#include <cmath>
#include <iostream>

#include "SteepestDescentSolver.hxx"

#ifndef ENABLETRACE_STEEPESTDESCENT
    #ifdef TRACESTEEPESTDESCENT
        #define ENABLETRACE_STEEPESTDESCENT true
    #else
        #define ENABLETRACE_STEEPESTDESCENT false
    #endif // TRACESTEEPESTDESCENT
#endif // ENABLETRACE_STEEPESTDESCENT
#ifndef METHODHEADER_STEEPESTDESCENT
    #define METHODHEADER_STEEPESTDESCENT Tracer _t_(__PRETTY_FUNCTION__, ENABLETRACE_STEEPESTDESCENT)
#endif // METHODHEADER_STEEPESTDESCENT

using namespace std;

SteepestDescentSolver::SteepestDescentSolver(){

}

SteepestDescentSolver::~SteepestDescentSolver(){
    //dtor
}

void SteepestDescentSolver::CalculateIteration(){
    METHODHEADER_STEEPESTDESCENT;
    m_iterationCounter++;

    double a,b,h;

    //Get new temp values and H
    a = m_hprimeX(m_guessX, m_guessY);
    b = m_hprimeY(m_guessX, m_guessY);
    h = m_hsolver(m_guessX, m_guessY, a, b);

    m_currentX = m_guessX + h*a;
    m_currentY = m_guessY + h*b;

    //Get the difference between the two values (We;re trying to get to
    //  a point where there isn't much between the two.
    m_iterDiff = sqrt(pow(m_currentX-m_guessX, 2.0) +
                        pow(m_currentY-m_guessY, 2.0));

    m_guessX = m_currentX;
    m_guessY = m_currentY;
}

void SteepestDescentSolver::IterateUntilSolved(){
    METHODHEADER_STEEPESTDESCENT;

    while(m_iterDiff > m_diffTolerance ){
        CalculateIteration();
#if DEBUG
        PrintDebug(cerr,"Printing [DEBUG] block ...");
        PrintPrettyResults(cerr);
        PrintDebug(cerr,"... End of [DEBUG] block.");
#endif // DEBUG
    }
}

void SteepestDescentSolver::PrintPrettyResults(ostream& out)const{
    METHODHEADER_STEEPESTDESCENT;

    out << "\nSteepest Descent Solver Results:" << endl;
    out << "(X,Y): (" << m_currentX << " , " << m_currentY <<  ")" << endl;
    out << "Difference: " << m_iterDiff << endl;
    out << "Iteration Count: " << m_iterationCounter << '\n' << endl;
}

bool SteepestDescentSolver::SolveCompletely(){
    bool result = false;
    try{
        if (Validate())
            IterateUntilSolved();


        result = true;
    }catch(exception ex){
        m_failMessage += ex.what();
    }
    return result;
}

bool SteepestDescentSolver::Validate(){
    bool result = true;

    if (m_hprimeX == nullptr || m_hprimeY == nullptr || m_hsolver == nullptr){
        m_failMessage += "Missing delegate method HPrimeX, HPrimeY, or HSolver;";
        result = false;
    }

    return result;
}
