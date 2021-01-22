import traceback

import src.matrix_operations as matops
import src.solvers as solvers
from src.utils import eprint


def add_subparser(subparsers):
    parser = subparsers.add_parser(
        "solve", help="Calculate solutions to matrix problems using a given method"
    )
    parser.add_argument(
        "dir", metavar="DIR", help="The directory containing matrix definitions."
    )
    parser.add_argument(
        "solver", choices=solvers.get_solver_list(), help="The solver method to use."
    )
    parser.add_argument(
        "--check",
        action="store_true",
        help="Compare the calculated result with the soln.def file.",
    )


def main(options):
    matA, matb, matsoln = matops.load_files(options.dir)
    solver = solvers.get_solver_instance_by_name(options.solver, matA=matA, matb=matb)

    if solver.solve():
        res = solver.result.reshape((-1, 1))
        err_norm = matops.two_norm_of_error(matA, matb, res)
        _solve_print_results(solver.get_solver_name(), res, err_norm)

        if options.check:
            if not matops.almost_equal(res, matsoln):
                err_percent = matops.percent_error(res, matsoln)
                eprint("Calculated result does not match expected solution.")
                eprint("Two norm of error: %.6f" % err_norm)
                eprint("Percent error: %.6f" % err_percent)
                eprint("Files: %s" % options.dir)
                return False

        return _solve_check_two_norm_within_range(err_norm)

    else:
        # TODO replace this with something better
        eprint("Solver failed")
        eprint(solver.result)
        eprint(traceback.print_tb(solver.result.__traceback__))
        return False


def _solve_check_two_norm_within_range(norm_err):
    l = 0.0
    u = 0.0005

    if not (l <= norm_err <= u):
        eprint('Two norm of err "%.6f" not in range [%f, %f]' % (norm_err, l, u))
        return False
    return True


def _solve_print_results(solver_name, result, norm_err):
    print("\n%s solver succeeded. Result matrix:" % solver_name)
    print(result)
    print("\nTwo norm of error: %.6f" % norm_err)
