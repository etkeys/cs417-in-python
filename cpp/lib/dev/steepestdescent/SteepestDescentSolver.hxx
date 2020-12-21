#ifndef STEEPESTDESCENTSOLVER_HXX
#define STEEPESTDESCENTSOLVER_HXX

#include <functional>
#include <iosfwd>
#include <string>

#include "Solver.hxx"

/*
PLEASE NOTE:
This code style looks very different than other code of mine
because code blocks provided a code generator (YAY!!) since
was the first chance I get to use it. It did an alright job.
*/

class SteepestDescentSolver
{
    public:
        SteepestDescentSolver();
        virtual ~SteepestDescentSolver();
        uint GetIterationCounter() { return m_iterationCounter; }

        double GetGuessX() { return m_guessX; }
        void SetGuessX(double val) { m_guessX = val; }
        double GetGuessY() { return m_guessY; }
        void SetGuessY(double val) { m_guessY = val; }
        double GetCurrentX() { return m_currentX; }
        void SetCurrentX(double val) { m_currentX = val; }
        double GetCurrentY() { return m_currentY; }
        void SetCurrentY(double val) { m_currentY = val; }
        void SetDiffTolerance(double val) { m_diffTolerance = val; }
        std::string GetFailMessage() {return m_failMessage;}
        void SetHPrimeXDelegate(std::function<double(double,double)> val) { m_hprimeX = val; }
        void SetHPrimeYDelegate(std::function<double(double,double)> val) { m_hprimeY = val; }
        void SetHSolverDelegate(std::function<double(double,double, double, double)> val) { m_hsolver = val; }

        void CalculateIteration();
        void IterateUntilSolved();
        void PrintPrettyResults(std::ostream&)const;
        bool SolveCompletely();
        bool Validate();

    protected:
    private:
        double m_diffTolerance = 0.01;
        uint m_iterationCounter = 0;
        double m_guessX = 1;
        double m_guessY = 1;
        double m_iterDiff = 1; //arbitrary difference, just something bigger than tolerance
        double m_currentX = 0;
        double m_currentY = 0;
        std::string m_failMessage = "";
        std::function<double(double,double)> m_hprimeX = nullptr;
        std::function<double(double,double)> m_hprimeY = nullptr;
        std::function<double(double,double, double, double)> m_hsolver = nullptr;
};

#endif // STEEPESTDESCENTSOLVER_HXX
