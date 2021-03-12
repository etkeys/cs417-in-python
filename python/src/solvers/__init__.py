from ._base import IterativeInitialGuess, Result, ResultAttributes
from .gauss_seidel import GaussSeidelSolver
from .gaussian import GaussianSolver
from .jacobi import JacobiSolver
from .ludecomposition import LuDecompositionSolver
from .sor_solver import SORSolver


def _get_solver_classes():
    """Returns dict collection of solver names to static class reference"""
    from ._base import _BasicSolver

    def is_public(s: str):
        return False if s.__name__.startswith("_") else True

    def collect_subclasses(seed):
        subclasses = {}
        for subclass in seed.__subclasses__():
            if is_public(subclass):
                subclasses[subclass.get_solver_name().lower()] = subclass
            subclasses = {**subclasses, **(collect_subclasses(subclass))}
        return subclasses

    return collect_subclasses(_BasicSolver)


def get_solver_instance(name, inputs):
    solvers = _get_solver_classes()
    if name not in solvers:
        raise KeyError(f'Solver "{name}" does not exist.')
    return solvers[name](**inputs)


def get_solver_list():
    solvers = sorted(list(_get_solver_classes().keys()))
    return solvers
