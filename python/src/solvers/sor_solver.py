
from .solver import _IterativeSolver, IterativeInitialGuess

import src.matrix_operations as matops

class SORSolver(_IterativeSolver):
    """
    Yes, SOR is a varient of Gauss-Seidel, but implementation of some mechanics
    make inheritance more trouble than helpful.
    """

    # TODO inherit from Jacobi to reuse most of __init__
    # then add handling here for omega
    def __init__(self, matA, matb, guess_source: IterativeInitialGuess, omega: float):
        self._allowed_guess_sources = [
            IterativeInitialGuess.DEFAULT,
            IterativeInitialGuess.MATRIX_OF_ZEROS,
            IterativeInitialGuess.RANDOM_MATRIX,
        ]
        super().__init__(matA, matb, guess_source)
        if guess_source == IterativeInitialGuess.DEFAULT:
            self._guess_source = IterativeInitialGuess.MATRIX_OF_ZEROS

        try:
            if omega is None:
                raise ValueError("Omega cannot be None")
            if not (0.0 < omega < 2.0):
                raise ValueError("Omega must be in the interval (0,2), but here is %f" % omega)
            self._omega = omega
        except TypeError:
            raise TypeError("Omega must be castable to type float, here is %s" % type(omega).__name__)

    @staticmethod
    def get_solver_name():
        return "SOR"

    def _build_interim_matricies(self):
        self._matD = matops.create_based_on_diagonal_terms(self._matA)
        
        matL = matops.create_based_on_l_component(self._matA)
        self._matL = matops.subtract(matL, self._matD)

        self._matU = matops.create_based_on_u_component(self._matA)

    def _calculate_iteration_result(self, mat_guess):
        # When converting from CPP code, found out that the CPP
        # implmentation is wrong. Below is the correct implemenation...
        # ret = ((D + wL)^-1) * (wb - (wU + (w-1)D)g)
        # Rewritte:
        #     ret = T1 * T2
        #     T1 = (D + wL)^-1
        #     T2 = T3 - T4
        #     T3 = wb
        #     T4 = T5 * g
        #     T5 = T6 + T7
        #     T6 = wU
        #     T7 = (w-1)D
        b = self._matb
        D = self._matD
        g = mat_guess
        L = self._matL
        U = self._matU
        w = self._omega

        T1 = matops.multiply(L, w)          # wL
        T1 = matops.add(D, T1)              # D + wL
        T1 = matops.create_inverted(T1)     # (D + wL)^-1

        T7 = matops.multiply(D, (w - 1))    # (w-1)D
        T6 = matops.multiply(U, w)          # wU
        T5 = matops.add(T6, T7)             # wU + (w-1)D
        T4 = matops.multiply(T5, g)         # (wU + (w-1)D)g
        T3 = matops.multiply(b, w)          # wb
        T2 = matops.subtract(T3, T4)        # wb - ((wU + (w-1)D)g

        ret = matops.multiply(T1,T2)        # ((D + wL)^-1) * (wb - (wU + (w-1)D)g)

        return ret

    # TODO inherit from Jacobi to reuse _create_guess
    # Make sure to change _matC to _matb
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
