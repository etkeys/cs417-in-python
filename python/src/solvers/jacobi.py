from ._base import IterativeInitialGuess, _IterativeSolver
import src.matrix_operations as matops


class JacobiSolver(_IterativeSolver):
    def __init__(self, **kwargs):
        """
        :key matA: Matrix A
        :key matb: Matrix b
        :key guess_source: how guess should be derived, should be an IterativeInitialGuess
        """
        self._allowed_guess_sources = [
            IterativeInitialGuess.DEFAULT,
            IterativeInitialGuess.MATRIX_OF_ZEROS,
            IterativeInitialGuess.RANDOM_MATRIX,
        ]
        super().__init__(**kwargs)

        if self._guess_source == IterativeInitialGuess.DEFAULT:
            self._guess_source = IterativeInitialGuess.MATRIX_OF_ZEROS

    @staticmethod
    def get_solver_name():
        return "Jacobi"

    def _build_interim_matricies(self):
        matC = matops.create_based_on_non_diagonal_terms(self._matA)

        matD = matops.create_based_on_diagonal_terms(self._matA)
        matD = matops.create_inverted(matD)

        self._matC = matC
        self._matD = matD

    def _calculate_iteration_result(self, mat_guess):
        if not matops.is_vector(mat_guess):
            raise ValueError("Guess matrix must be a vector")

        cg = matops.multiply(self._matC, mat_guess)
        b_less_cg = matops.subtract(self._matb, cg)
        ret = matops.multiply(self._matD, b_less_cg)

        return ret

    def _create_guess(self):
        if not hasattr(self, "_matD"):
            self._build_interim_matricies()
        ret = (
            matops.create_random(matops.count_rows(self._matb), True)
            if self._guess_source == IterativeInitialGuess.RANDOM_MATRIX
            else matops.create_zeros(matops.count_rows(self._matb))
        )
        ret = matops.reshape(ret, (-1, 1))
        return ret
