from ._base import IterativeInitialGuess, Result, ResultAttributes
from .gauss_seidel import GaussSeidelSolver
from .gaussian import GaussianSolver
from .jacobi import JacobiSolver
from .ludecomposition import LuDecompositionSolver
from .sor_solver import SORSolver


def get_solver_instance(inputs, options):
    if options is None:
        raise ValueError("options argument not given.")

    # TODO these might not be a requirement for all solvers
    if not "matA" in inputs:
        raise KeyError("Matrix A not provided.")
    if not "matb" in inputs:
        raise KeyError("Matrix b not provided.")

    name = options.solver

    # TODO Can this name lookup be done via dictionary?
    # For example:
    #   mapping = {'gaussian': _create_gaussian_instance}
    # Or by a static method, create_instance, available on all solvers?
    if "gaussian" == name:
        return GaussianSolver(inputs["matA"], inputs["matb"])
    elif "ludecomposition" == name:
        return LuDecompositionSolver(inputs["matA"], inputs["matb"])
    elif JacobiSolver.get_solver_name().lower() == name:
        return JacobiSolver(
            inputs["matA"],
            inputs["matb"],
            IterativeInitialGuess.from_string(options.guess),
        )
    elif GaussSeidelSolver.get_solver_name().lower() == name:
        return GaussSeidelSolver(
            inputs["matA"],
            inputs["matb"],
            IterativeInitialGuess.from_string(options.guess),
        )
    elif SORSolver.get_solver_name().lower() == name:
        return SORSolver(
            inputs["matA"],
            inputs["matb"],
            IterativeInitialGuess.from_string(options.guess),
            inputs.get("omega", options.omega),
        )
    else:
        raise ValueError('Solver "%s" has no create implementation.' % name)


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
