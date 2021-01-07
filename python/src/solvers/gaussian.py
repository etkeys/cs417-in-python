from .solver import _Solver
import src.matrix_operations as matops


class GaussianSolver(_Solver):
    """
    Gauss-Jordan elimination
    """

    def __init__(self, aug_matrix):
        if aug_matrix is None:
            raise ValueError("Provided augmented matrix must not be None.")
        self._mat = aug_matrix

    def solve(self):
        try:
            self._mat = matops.to_reduced_row_echelon(self._mat)
            self._result = self._calculate_back_solve_vector()
            return True
        except Exception as ex:
            self._result = ex
            return False

    def _calculate_back_solve_vector(self):
        if matops.is_singular(self._mat):
            raise ValueError("Provided matrix is singular.")

        if not matops.is_in_reduced_row_echelon(self._mat):
            self._mat = matops.to_reduced_row_echelon(self._mat)

        ret = matops.create_zeros(self._mat.shape[0])
        for r in range(ret.shape[0] - 1, -1, -1):
            ret[r] = self._calculate_back_solve_vector_row(ret, r)

        return ret

    def _calculate_back_solve_vector_row(self, bsvec, calc_row):
        # Give an augmented matrix:
        #   [[M00, M01, M02, M03, M04]
        #    [M10, M11, M12, M13, M14]
        #    [M20, M21, M22, M23, M24]
        #    [M30, M31, M32, M33, M34]]
        #
        # And given a backsolve vector:
        #   [R0, R1, R2, R3]
        #
        # To calculate any backsolve vector row:
        # Rr = Mrc - (Mr(c-1) * R(rMAX)) - ... - (Mr(1) * R(r-1))
        #   for any row (r)
        #   for any column (c), where c in [cMAX..1]
        #
        # R3 = M[3,4]
        # R2 = M[2,4] - (M[2,3] * R[3])
        # R1 = M[1,4] - (M[1,3] * R[3]) - (M[1,2] * R[2])
        # R0 = M[0,4] - (M[0,3] * R[3]) - (M[0,2] * R[2]) - (M[0,1] * R[1])

        cmax = self._mat.shape[1] - 1
        seed = self._mat[calc_row, cmax]
        # print('Seed=%.6f' % seed)

        subs = [
            (self._mat[calc_row, index] * bsvec[index])
            for index in range(cmax - 1, calc_row, -1)
        ]
        # print(subs)

        return seed - sum(subs)
