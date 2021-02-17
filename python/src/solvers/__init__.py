from .gaus_seidel import GausSeidelSolver
from .gaussian import GaussianSolver
from .jacobi import JacobiSolver
from .ludecomposition import LuDecompositionSolver
from .solver import ComplexResult, IterativeInitialGuess


def get_solver_instance(options, **kwargs):
    if options is None:
        raise ValueError("options argument not given.")

    # TODO these might not be a requirement for all solvers
    if not "matA" in kwargs:
        raise KeyError("Matrix A not provided.")
    if not "matb" in kwargs:
        raise KeyError("Matrix b not provided.")

    name = options.solver

    # TODO Can this name lookup be done via dictionary?
    # For example:
    #   mapping = {'gaussian': _create_gaussian_instance}
    # Or by a static method, create_instance, available on all solvers?
    if "gaussian" == name:
        return GaussianSolver(kwargs["matA"], kwargs["matb"])
    elif "ludecomposition" == name:
        return LuDecompositionSolver(kwargs["matA"], kwargs["matb"])
    elif JacobiSolver.get_solver_name().lower() == name:
        return JacobiSolver(
            kwargs["matA"],
            kwargs["matb"],
            IterativeInitialGuess.from_string(options.guess),
        )
    else:
        raise ValueError('Solver "%s" has no create implementation.' % name)


def get_solver_list():
    from .solver import _Solver

    def is_public(s: str):
        return False if s.__name__.startswith("_") else True

    def collect_subclasses(seed):
        subclasses = []
        for subclass in seed.__subclasses__():
            if is_public(subclass):
                subclasses.append(subclass.get_solver_name().lower())
            subclasses.extend(collect_subclasses(subclass))
        return subclasses

    solvers = sorted(collect_subclasses(_Solver))
    return solvers
