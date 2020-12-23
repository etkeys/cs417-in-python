import argparse

import matrix as Matrix
import solvers

def main(options):
    if options.subcommand == 'make':
        make(options)
    elif options.subcommand == 'solve':
        solve(options)

# def main(options):
#     if options.subcommand == 'make':
#         make(options)

def make(options):
    a = Matrix.create_random_diagonal_dominate(options.size)
    soln = Matrix.create_random(options.size, True)
    b = Matrix.multiply(a, soln)

    tdir = Matrix.write_files(
        (a, 'A.def'),
        (b, 'b.def'),
        (soln, 'soln.def'))
    print(tdir)

def solve(options):
    A, b, _ = Matrix.load_files(options.dir)
    aug = Matrix.create_augmented(A, b)

    solver = solvers.GaussianSolver(aug)
    if (solver.solve()):
        res = solver.result.reshape((-1, 1))
        print('\nGaussian solver succeeded. Result matrix:')
        print(res)
        err = Matrix.two_norm_of_error(A, b, res)
        print('\nTwo norm of the error: %.6f' % err)
    else:
        # TODO replace this with something better
        print("Solver failed")


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest='subcommand', help='sub-command help.')

    parser_a = subparsers.add_parser('make', help='Make matrix help')
    parser_a.add_argument('size', type=int, help='Size of matrix A')

    parser_b = subparsers.add_parser('solve', help='Solve matrix help')
    parser_b.add_argument('dir', help='The directory containing matrix definitions.')

    options = parser.parse_args()
    main(options)