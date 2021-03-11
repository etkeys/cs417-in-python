from ._base import IterativeInitialGuess, _IterativeSolver
import src.matrix_operations as matops


class GaussSeidelSolver(_IterativeSolver):
    def __init__(self, matA, matb, guess_source: IterativeInitialGuess):
        super().__init__(matA, matb, guess_source)
        if guess_source == IterativeInitialGuess.DEFAULT:
            self._guess_source = IterativeInitialGuess.MATRIX_C

    @staticmethod
    def get_solver_name():
        return "GaussSeidel"

    def _build_interim_matricies(self):
        matL = matops.create_based_on_l_component(self._matA)
        matL = matops.create_inverted(matL)
        matU = matops.create_based_on_u_component(self._matA)
        self._matC = matops.multiply(matL, self._matb)
        self._matD = matops.multiply(matL, matU) * -1.0

    def _calculate_iteration_result(self, mat_guess):
        if not matops.is_vector(mat_guess):
            raise ValueError("Guess matrix must be a vector")
        dg = matops.multiply(self._matD, mat_guess)
        dgc = matops.add(dg, self._matC)
        return dgc

    def _create_guess(self):
        if not hasattr(self, "_matC"):
            self._build_interim_matricies()
        rows = matops.count_rows(self._matb)
        creators = {
            IterativeInitialGuess.MATRIX_C: lambda: matops.deepcopy(self._matC),
            IterativeInitialGuess.RANDOM_MATRIX: lambda: matops.create_random(
                rows, True
            ),
            IterativeInitialGuess.MATRIX_OF_ZEROS: lambda: matops.create_zeros(rows, 1),
        }
        return creators.get(self._guess_source)()
