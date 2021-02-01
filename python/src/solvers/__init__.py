from .gaussian import GaussianSolver
from .jacobi import JacobiSolver
from .ludecomposition import LuDecompositionSolver
from .solver import ComplexResult


def get_solver_instance(cli_options, **kwargs):
    if cli_options is None:
        raise ValueError("cli_options argument not given.")
    if not hasattr(cli_options, "solver"):
        raise TypeError('cli_options missing attribute "solver".')
    if not isinstance(cli_options.solver, str):
        raise TypeError("cli_options.solver value is not type str.")
    if "jacobi" == cli_options.solver:
        return JacobiSolver.create(cli_options, **kwargs)
    else:
        return get_solver_instance_by_name(cli_options.solver, **kwargs)


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
