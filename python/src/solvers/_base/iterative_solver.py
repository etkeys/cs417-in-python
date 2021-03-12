from math import isclose
from typing import Type

from .basic_solver import _BasicSolver
from .enumerations import IterativeInitialGuess, ResultAttributes

from src.exceptions import DivergentSolution
import src.matrix_operations as matops


class _IterativeSolver(_BasicSolver):
    _ITER_MAX = 1000

    _allowed_guess_sources = [
        IterativeInitialGuess.DEFAULT,
        IterativeInitialGuess.MATRIX_C,
        IterativeInitialGuess.MATRIX_OF_ZEROS,
        IterativeInitialGuess.RANDOM_MATRIX,
    ]

    def __init__(self, **kwargs):
        """
        :key matA: Matrix A
        :key matb: Matrix b
        :key guess_source: how guess should be derived, should be an IterativeInitialGuess
        """
        super().__init__(**kwargs)

        if "guess_source" not in kwargs:
            raise KeyError("Solver not given guess source.")

        guess_source = kwargs["guess_source"]
        if not isinstance(guess_source, IterativeInitialGuess):
            if isinstance(guess_source, str):
                guess_source = IterativeInitialGuess.from_string(guess_source)
            else:
                raise TypeError(
                    f"Cannot convert {guess_source} to proper guess source because it is {type(guess_source).__name__}."
                )

        if guess_source not in self._allowed_guess_sources:
            raise ValueError(
                f"Guess choice {guess_source} is not valid in this context."
            )

        self._guess_source = guess_source
        self._matb = (
            self._matb
            if matops.is_vvector(self._matb)
            else matops.reshape(self._matb, (-1, 1))
        )

    def solve(self):
        try:
            self._build_interim_matricies()
            guess = self._create_guess()

            res, iter_count = self._iterate_until_solved(guess)
            self._result[ResultAttributes.RESULT_VECTOR] = res
            self._result[ResultAttributes.ITERATIONS] = iter_count

            if iter_count > self._ITER_MAX:
                raise DivergentSolution(self.get_solver_name())

        except Exception as ex:
            self._result[ResultAttributes.ERROR] = ex
            return False
        return True

    def _build_interim_matricies(self):
        raise NotImplementedError(
            "_IterativeSolver does not implement _build_interim_matricies()"
        )

    def _calculate_iteration_result(self, mat_guess):
        raise NotImplementedError(
            "_IterativeSolver does not implement _calculate_iteration_result()"
        )

    def _create_guess(self):
        raise NotImplementedError("_IterativeSolver does not implement _create_guess()")

    def _iterate_until_solved(self, mat_guess):
        two_norm = 1.0
        iter_count = 0
        ret = matops.deepcopy(mat_guess)
        while self._should_iteration_continue(iter_count, two_norm):
            ret = self._calculate_iteration_result(ret)
            two_norm = matops.two_norm_of_error(self._matA, self._matb, ret)
            iter_count += 1

        return (ret, iter_count)

    def _should_iteration_continue(self, iter_count, two_norm):
        tol = 0.000_05
        if isclose(two_norm, 0.0, abs_tol=tol):
            return False
        elif iter_count > self._ITER_MAX:
            return False
        else:
            return True
