from .result import Result
import src.matrix_operations as matops


class _BasicSolver(object):
    def __init__(self, **kwargs):
        """
        :key matA: Matrix A
        :key matb: Matrix b
        """
        if "matA" not in kwargs:
            raise KeyError("Solver not given Matrix A.")
        if "matb" not in kwargs:
            raise KeyError("Solver not given Matrix b.")

        if not matops.is_matrix(kwargs["matA"]):
            raise ValueError("Matrix A missing or not a matrix.")
        if not matops.is_square(kwargs["matA"]):
            raise ValueError("Matrix A is not square.")
        if not matops.is_vector(kwargs["matb"]):
            raise ValueError("Matrix b missing or not a vector.")

        self._matA = kwargs["matA"]
        self._matb = (
            kwargs["matb"]
            if matops.is_hvector(kwargs["matb"])
            else matops.reshape(kwargs["matb"], -1)
        )
        self._result = Result()

    @staticmethod
    def get_solver_name():
        raise NotImplementedError("_Solver does not implement get_solver_name().")

    def solve(self):
        raise NotImplementedError("_Solver does not implement solve().")

    @property
    def result(self):
        return self._result
