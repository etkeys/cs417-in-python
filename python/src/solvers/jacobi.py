from enum import Enum
from math import isclose

from .solver import _ComplexResultSolver, ComplexResult
import src.matrix_operations as matops


class InitialGuess(Enum):
    MATRIX_OF_ZEROS = 1
    MATRIX_C = 2
    RANDOM_MATRIX = 3


class JacobiSolver(_ComplexResultSolver):
    _ITER_ROUNDS_MAX = 1000

    def __init__(self, matA, matb, guess_source: InitialGuess = InitialGuess.MATRIX_C):
        super().__init__()
        if not matops.is_matrix(matA):
            raise ValueError("Matrix A missing or not a matrix.")
        if not matops.is_square(matA):
            raise ValueError("Matrix A is not square.")
        if not matops.is_vector(matb):
            raise ValueError("Matrix b missing or not a vector.")
        if guess_source not in [
            InitialGuess.RANDOM_MATRIX,
            InitialGuess.MATRIX_OF_ZEROS,
        ]:
            raise ValueError("Guess choice can only be random or zero")

        self._guess_source = guess_source
        self._matA = matops.deepcopy(matA)
        self._matb = matops.deepcopy(
            matb if matops.is_vvector(matb) else matops.reshape(matb, (-1, 1))
        )

    @staticmethod
    def create(cli_options, **kwargs):
        if not hasattr(cli_options, "guess"):
            raise TypeError('cli_options missing attribute "guess"')
        choices = ["default", "random", "zeros"]
        if cli_options.guess not in choices:
            raise ValueError(
                "cli_options.guess value is {}, but expected any of these: {}".format(
                    cli_options.guess, choices
                )
            )
        guess = (
            InitialGuess.RANDOM_MATRIX
            if "random" == cli_options.guess
            else InitialGuess.MATRIX_OF_ZEROS
        )

        return JacobiSolver(kwargs["matA"], kwargs["matb"], guess)

    @staticmethod
    def get_solver_name():
        return "Jacobi"

    def solve(self):
        _RA = ComplexResult.Attributes
        try:
            self._matC, self._matD = self._create_interim_matricies()
            res, iter_count = self._iterate_until_solved()
            self._result[_RA.RESULT_VECTOR] = res
            self._result[_RA.ITERATIONS] = iter_count
            if iter_count > self._ITER_ROUNDS_MAX:
                self._result[_RA.CAVEATS] = ComplexResult.Caveats.LAME_CONVERGANCE
        except Exception as ex:
            self._result[_RA.ERROR] = ex
            self._result[_RA.CAVEATS] = ComplexResult.Caveats.ERROR
            return False
        return True

    def _calculate_iteration_result(self, mat_guess):
        if not matops.is_vector(mat_guess):
            raise ValueError("Guess matrix must be a vector")

        cg = matops.multiply(self._matC, mat_guess)
        b_less_cg = matops.subtract(self._matb, cg)
        ret = matops.multiply(self._matD, b_less_cg)

        return ret

    def _create_interim_matricies(self):
        ret_matC = matops.create_based_on_non_diagonal_terms(self._matA)

        ret_matD = matops.create_based_on_diagonal_terms(self._matA)
        ret_matD = matops.create_inverted(ret_matD)

        return (ret_matC, ret_matD)

    def _iterate_until_solved(self):
        def should_continue(iter_count, err):
            tol = 0.000_05
            if isclose(err, 0.0, abs_tol=tol):
                return False
            elif iter_count > self._ITER_ROUNDS_MAX:
                return False
            else:
                return True

        ret = (
            matops.create_random(matops.count_rows(self._matC), True)
            if self._guess_source == InitialGuess.RANDOM_MATRIX
            else matops.create_zeros(matops.count_rows(self._matC))
        )
        ret = matops.reshape(ret, (-1, 1))

        twoNorm = 1.0
        iter_count = 0
        while should_continue(iter_count, twoNorm):
            ret = self._calculate_iteration_result(ret)
            twoNorm = matops.two_norm_of_error(self._matA, self._matb, ret)
            iter_count += 1

        return (ret, iter_count)
