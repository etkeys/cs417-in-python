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
        "solver",
        choices=solvers.get_solver_list(),
        type=str,
        help="The solver method to use.",
    )
    parser.add_argument(
        "--check",
        action="store_true",
        help="Compare the calculated result with the soln.def file.",
    )
    parser.add_argument(
        "--guess",
        action="store",
        choices=solvers.IterativeInitialGuess.as_strings(),
        default=solvers.IterativeInitialGuess.to_string(
            solvers.IterativeInitialGuess.DEFAULT
        ),
        type=str,
        help="Specifies the initial guess type for iterative solvers.",
    )
    parser.add_argument(
        "--omega",
        action="store",
        default=1.0,
        type=float,
        help="Specifies the relaxation factor for SOR solver.",
    )
    parser.add_argument(
        "-q",
        "--quite",
        action="store_true",
        default=False,
        dest="quite",
        help="Do not print the result vector when printing results.",
    )


def main(options):
    inputs = matops.load_files(options.dir)
    solver = solvers.get_solver_instance(inputs, options)

    if solver.solve():
        result = solver.result
        res_vec = result[solvers.ResultAttributes.RESULT_VECTOR]

        add_msgs = []
        if solvers.ResultAttributes.ITERATIONS in result:
            add_msgs = [f"Iterations: {result[solvers.ResultAttributes.ITERATIONS]}"]

        err_norm = matops.two_norm_of_error(inputs["matA"], inputs["matb"], res_vec)
        _solve_print_results(
            solver.get_solver_name(), res_vec, err_norm, options.quite, *add_msgs
        )

        if options.check:
            if not matops.almost_equal(res_vec, inputs["matsoln"]):
                err_percent = matops.percent_error(res_vec, inputs["matsoln"])
                eprint("Calculated result does not match expected solution.")
                eprint("Two norm of error: %.6f" % err_norm)
                eprint("Percent error: %.6f" % err_percent)
                eprint("Files: %s" % options.dir)
                return False

        return _solve_check_two_norm_within_range(err_norm)

    else:
        err = solver.result[solvers.ResultAttributes.ERROR]
        eprint("%s solver failed!" % solver.get_solver_name())
        eprint(err)
        eprint(traceback.print_tb(err.__traceback__))
        return False


def _solve_check_two_norm_within_range(norm_err):
    l = 0.0
    u = 0.0005

    if not (l <= norm_err <= u):
        eprint('Two norm of err "%.6f" not in range [%f, %f]' % (norm_err, l, u))
        return False
    return True


def _solve_print_results(solver_name, result_vec, norm_err, quite, *args):
    print("%s solver succeeded!" % solver_name)
    if not quite:
        print("Result matrix:\n{}".format(result_vec))
    print("Two norm of error: %.6f" % norm_err)
    for msg in args:
        print(msg)
