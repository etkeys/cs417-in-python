class DivergentSolution(Exception):
    """Raised when a calculated solution does not converge"""

    def __init__(self, solver_name, *args: object) -> None:
        super().__init__(("%s solution is divergent!" % solver_name), *args)
