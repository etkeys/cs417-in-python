
import src.matrix_operations as matops
import src.solvers as solvers
from src.utils import eprint

def add_subparser(subparsers):
    parser = subparsers.add_parser('solve',
        help='Calculate solutions to matrix problems using a given method')
    parser.add_argument('dir', metavar='DIR', help='The directory containing matrix definitions.')
    parser.add_argument('--check',
        action='store_true',
        help='Compare the calculated result with the soln.def file.')

def main(options):
    A, b, soln = matops.load_files(options.dir)
    aug = matops.create_augmented(A, b)

    solver = solvers.GaussianSolver(aug)
    if (solver.solve()):
        res = solver.result.reshape((-1, 1))
        err = matops.two_norm_of_error(A, b, res)
        _solve_print_results(res, err)

        if options.check:
            if not matops.almost_equal(res, soln):
                eprint('Calculated result does not match expected solution.')
                eprint("Two norm of error: %.6f" % err)
                return False

        return _solve_check_two_norm_within_range(err)

    else:
        # TODO replace this with something better
        eprint("Solver failed")
        print(solver.result)
        return False

def _solve_check_two_norm_within_range(norm_err):
    l = 0.0
    u = 0.0005

    if not (l <= norm_err <= u):
        eprint('Two norm of err "%.6f" not in range [%f, %f]' % (norm_err, l, u))
        return False
    return True

def _solve_print_results(result, norm_err):
    print('\nGaussian solver succeeded. Result matrix:')
    print(result)
    print('\nTwo norm of the error: %.6f' % norm_err)
