from ._base import IterativeInitialGuess, Result, ResultAttributes
from .gauss_seidel import GaussSeidelSolver
from .gaussian import GaussianSolver
from .jacobi import JacobiSolver
from .ludecomposition import LuDecompositionSolver
from .sor_solver import SORSolver


def get_solver_instance(name, inputs):
    # TODO Can this name lookup be done via dictionary?
    # For example:
    #   mapping = {'gaussian': _create_gaussian_instance}
    # Or by a static method, create_instance, available on all solvers?
    if "gaussian" == name:
        return GaussianSolver(**inputs)
    elif "ludecomposition" == name:
        return LuDecompositionSolver(**inputs)
    elif JacobiSolver.get_solver_name().lower() == name:
        return JacobiSolver(**inputs)
    elif GaussSeidelSolver.get_solver_name().lower() == name:
        return GaussSeidelSolver(**inputs)
    elif SORSolver.get_solver_name().lower() == name:
        return SORSolver(**inputs)
    else:
        raise ValueError(f'Solver "{name}" has no create implementation.')


def get_solver_list():
    from ._base import _BasicSolver

    def is_public(s: str):
        return False if s.__name__.startswith("_") else True

    def collect_subclasses(seed):
        subclasses = []
        for subclass in seed.__subclasses__():
            if is_public(subclass):
                subclasses.append(subclass.get_solver_name().lower())
            subclasses.extend(collect_subclasses(subclass))
        return subclasses

    solvers = sorted(collect_subclasses(_BasicSolver))
    return solvers
