from math import fabs, isclose


from ._base import _BasicSolver, ResultAttributes
from .gaussian import GaussianSolver
import src.matrix_operations as matops


class LuDecompositionSolver(_BasicSolver):
    """
    Crout LU decomposition
    """

    def __init__(self, matA, matb):
        super().__init__()
        if not matops.is_matrix(matA):
            raise ValueError("Matrix A missing or not a matrix.")
        if not matops.is_square(matA):
            raise ValueError("Matrix A is not square.")
        if not matops.is_vector(matb):
            raise ValueError("Matrix b missing or not a vector.")

        self._matA = matA
        self._matb = matb if matops.is_hvector(matb) else matops.reshape(matb, -1)

    @staticmethod
    def get_solver_name():
        return "LuDecomposition"

    def solve(self):
        try:
            matL, matU = self._create_crout_lu_factors()
            maty = self._calculate_forward_solve_vector(matL)
        except Exception as ex:
            self._result[ResultAttributes.ERROR] = ex
            return False

        gs = GaussianSolver(matU, maty)
        ret = gs.solve()
        self._result = gs.result
        return ret

    def _calculate_forward_solve_vector(self, matL):
        if not matops.is_in_crout_l_form(matL):
            raise ValueError("Matrix L is missing or not in propert form.")

        size = matops.count_columns(self._matb)
        ret = matops.create_zeros(size)
        for index in range(size):
            ret[index] = self._calculate_y_vector_value(matL, ret[:index], index)

        return ret

    def _calculate_l_column(self, matL, matU, column, ubound):
        for row in range(column, ubound):
            row_sum = sum([matL[row, i] * matU[i, column] for i in range(column)])
            matL[row, column] = self._matA[row, column] - row_sum
        return matL

    def _calculate_u_row(self, matL, matU, row, ubound):
        # because of ldiv, calculating l columns and u rows cannot be done in
        # the same loop.  :(
        ldiv = matL[row, row]
        if isclose(fabs(ldiv), 0.0, rel_tol=0.000_01, abs_tol=0.000_01):
            raise ZeroDivisionError(
                "Matrix L[%i,%i] has 0-ish divisor: %.6f" % (row, row, ldiv)
            )

        for column in range(row, ubound):
            col_sum = sum([matL[row, i] * matU[i, column] for i in range(row)])
            matU[row, column] = (self._matA[row, column] - col_sum) / ldiv
        return matU

    def _calculate_y_vector_value(self, matL, vecy_part, index):
        ldiv = matL[index, index]
        if isclose(fabs(ldiv), 0.0, rel_tol=0.000_01, abs_tol=0.000_01):
            raise ZeroDivisionError(
                "Matrix L[%i,%i] has 0-ish divisor: %.6f" % (index, index, ldiv)
            )

        # print("Value from matb: %.6f" % self._matb[index])
        sub = matL[index, :index] * vecy_part
        # print("Values to sum and subtract from matb: {}".format(sub))
        ret = self._matb[index] - sub.sum()
        # print("Result of subtraction from matb: %.6f" % ret)
        # print("Value of ldiv: %.6f" % ldiv)
        ret /= ldiv
        # print("Final value to return: %.6f" % ret)
        return ret

    def _create_crout_lu_factors(self):
        size = matops.count_rows(self._matA)
        matL = matops.create_identity(size)
        matU = matops.create_identity(size)

        for index in range(size):
            matL = self._calculate_l_column(matL, matU, index, size)
            matU = self._calculate_u_row(matL, matU, index, size)

        return (matL, matU)


"""
NOTES

To create matrix L and matrix U, where L and U start as identity matricies
for matrix A...

Given matrix A:
[  2,  4,  1, -3]
[ -1, -2,  2,  4]
[  4,  2, -3,  5]
[  5, -4, -3,  1]

For L column 0
    L00 = A00
    L10 = A10
    L20 = A20
    L30 = A30

For L column 1
    L11 = A11 - (L10*U01)
    L21 = A21 - (L20*U01)
    L31 = A31 - (L30*U01)

For L column 2
    L22 = A22 - ((L20*U02) + (L21*U12))
    L32 = A32 - ((L30*U02) + (L31*U12))

For L column 3
    L33 = A33 - ((L30*U03) + (L31*U13) + (L32*U23))

For U row 0
    U00 = (A00) / L00
    U01 = (A01) / L00
    U02 = (A02) / L00
    U03 = (A03) / L00

For U row 1
    U11 = (A11 - (L10*U01)) / L11
    U12 = (A12 - (L10*U02)) / L11
    U13 = (A13 - (L10*U03)) / L11

For U row 2
    U22 = (A22 - ((L20*U02) + (L21*U12))) / L22
    U23 = (A23 - ((L20*U03) + (L21*U13))) / L22

For U row 3
    U33 = (A33 - ((L30*U03) + (L31*U13) + (L32*U23))) / L33
"""
