from .result import Result


class _BasicSolver(object):
    def __init__(self):
        self._result = Result()

    @staticmethod
    def get_solver_name():
        raise NotImplementedError("_Solver does not implement get_solver_name().")

    def solve(self):
        raise NotImplementedError("_Solver does not implement solve().")

    @property
    def result(self):
        return self._result
