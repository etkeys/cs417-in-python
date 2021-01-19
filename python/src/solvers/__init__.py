from .gaussian import GaussianSolver
from .ludecomposition import LuDecompositionSolver


def get_solver_instance_by_name(name: str = None, **kwargs):
    if not name:
        raise ValueError("Name argument must be a valid string.")

    # TODO these might not be a requirement for all solvers
    if not "matA" in kwargs:
        raise KeyError("Matrix A not provided.")
    if not "matb" in kwargs:
        raise KeyError("Matrix b not provided.")

    # TODO Can this name lookup be done via dictionary?
    # For example:
    #   mapping = {'gaussian': _create_gaussian_instance}
    # Or by a static method, create_instance, available on all solvers?
    if "gaussian" == name:
        return GaussianSolver(kwargs["matA"], kwargs["matb"])
    elif "ludecomposition" == name:
        return LuDecompositionSolver(kwargs["matA"], kwargs["matb"])
    else:
        raise ValueError('Solver "%s" has no create implementation.' % name)


def get_solver_list():
    from .solver import _Solver

    return [solver.get_solver_name().lower() for solver in _Solver.__subclasses__()]
