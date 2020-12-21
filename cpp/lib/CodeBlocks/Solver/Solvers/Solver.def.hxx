#ifndef SOLVER_DEF_HXX
#define SOLVER_DEF_HXX

template <class TSource, class TResult>
class Solver<TSource, TResult>{

public
    enum InputParameter {MatrixA, Matrixb, MatrixD, MatrixL, MatrixU};

    TResult* GetResult() = 0;
    void SetInput(TSource*, Solver::InputParameter);

}


#endif // SOLVER_DEF_HXX
