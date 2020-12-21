#ifndef SOLVER_HXX
#define SOLVER_HXX

#include "Solver.def.hxx"

#ifdef INCLUDESOLVER_GAUSSIAN
    #include "GaussianSolver.hxx"
    #ifdef INCLUDESOLVER_LU
        #include "LUDecompositionSolver.hxx"
    #endif
#endif

#ifdef INCLUDESOLVER_ITERATIVE
    #include "IterativeSolver.hxx"
#endif

#ifdef INCLUDESOLVER_NEWTON
    #include "NewtonSolver.hxx"
#endif

#ifdef INCLUDESOLVER_POWER
    #include "PowerSolver.hxx"
#endif

#ifdef INCLUDESOLVER_BISECTION
    #include "BisectionSolver.hxx"
#endif

#ifdef INCLUDESOLVER_EULER
    #include "EulerSolver.hxx"
#endif

#endif // SOLVER_HXX
