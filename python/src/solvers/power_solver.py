from ._base import _IterativeSolver, IterativeInitialGuess, ResultAttributes
import src.matrix_operations as matops

class PowerSolver(_IterativeSolver):

    def __init__(self, **kwargs):
        """
        :key matA: Matrix A
        :key guess_source: how guess should be derived, should be an IterativeInitialGuess
        """
        kwargs["matb"] = matops.create_zeros(1)
        self._allowed_guess_sources = [
            IterativeInitialGuess.DEFAULT,
            IterativeInitialGuess.MATRIX_OF_ZEROS,
            IterativeInitialGuess.RANDOM_MATRIX,
        ]
        super().__init__(**kwargs)

        if self._guess_source == IterativeInitialGuess.DEFAULT:
            self._guess_source = IterativeInitialGuess.RANDOM_MATRIX

    @staticmethod
    def get_solver_name():
        return "Power"

    def _build_interim_matricies(self):
        # We don't need to do this, but have to override else error is raised
        pass

    def _calculate_dominate_eigen_value(old, new):
        on_vec = matops.multiply(old, new)
        on_sum = matops.scalar_sum(on_vec)
        o2_vec = matops.multiply(old, old)
        o2_sum = matops.scalar_sum(o2_vec)

        ret = on_sum / o2_sum
        return ret

    def _calculate_iteration_result(self, mat_guess):
        ret = matops.multiply(self._matA, mat_guess)
        normalizer = matops.column_max(ret, 0)
        ret = matops.multiply(ret, (1/normalizer))
        return ret

    def _create_guess(self):
        ret = (
            matops.create_random(matops.count_rows(self._matA), True)
            if self._guess_source == IterativeInitialGuess.RANDOM_MATRIX
            else matops.create_zeros(matops.count_rows(self._matA))
        )
        ret = matops.reshape(ret, (-1, 1))
        return ret
    
    def _iteration_step(self, iter_count, mat_guess):
        ret = self._calculate_iteration_result(mat_guess)
        done = matops.almost_equal(mat_guess, ret)
        self._eigen = self._calculate_dominate_eigen_value(mat_guess, ret)

        iter_count += 1
        return (ret, done, iter_count)
    
    def _iterate_until_solved(self, mat_guess):
        ret, iter_count = super()._iterate_until_solved(mat_guess)

        self._result[ResultAttributes.EIGEN_VALUE] = self._eigen
        return (ret, iter_count)

    def _should_iteration_continue(self, iter_count, prev_new_equal):
        if iter_count > self._ITER_MAX:
            return False
        else:
            return not prev_new_equal
