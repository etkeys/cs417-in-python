import argparse
import sys

import matrix as Matrix
import solvers

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

def main(options):
    if options.subcommand == 'make':
        return make(options)
    elif options.subcommand == 'solve':
        return solve(options)

def make(options):
    a = Matrix.create_random_diagonal_dominate(options.size)
    soln = Matrix.create_random(options.size, True)
    b = Matrix.multiply(a, soln)

    tdir = Matrix.write_files(
        (a, 'A.def'),
        (b, 'b.def'),
        (soln, 'soln.def'))
    print(tdir)
    return True

def solve(options):
    A, b, soln = Matrix.load_files(options.dir)
    aug = Matrix.create_augmented(A, b)

    solver = solvers.GaussianSolver(aug)
    if (solver.solve()):
        res = solver.result.reshape((-1, 1))
        err = Matrix.two_norm_of_error(A, b, res)
        solve_print_results(res, err)

        if options.check:
            if not Matrix.almost_equal(res, soln):
                eprint('Calculated result does not match expected solution.')
                eprint("Two norm of error: %.6f" % err)
                return False

        return solve_check_two_norm_within_range(err)

    else:
        # TODO replace this with something better
        eprint("Solver failed")
        print(solver.result)
        return False

def solve_print_results(result, norm_err):
    print('\nGaussian solver succeeded. Result matrix:')
    print(result)
    print('\nTwo norm of the error: %.6f' % norm_err)

def solve_check_two_norm_within_range(norm_err):
    l = 0.0
    u = 0.0005

    if not (l <= norm_err <= u):
        eprint('Two norm of err "%.6f" not in range [%f, %f]' % (norm_err, l, u))
        return False
    return True

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest='subcommand', help='sub-command help.')

    parser_a = subparsers.add_parser('make', help='Make matrix help')
    parser_a.add_argument('size', type=int, help='Size of matrix A')

    parser_b = subparsers.add_parser('solve', help='Solve matrix help')
    parser_b.add_argument('dir', help='The directory containing matrix definitions.')
    parser_b.add_argument('--check',
        action='store_true',
        help='Compare the calculated result with the soln.def file.')

    options = parser.parse_args()
    if main(options):
        exit(0)
    else:
        exit(1)

