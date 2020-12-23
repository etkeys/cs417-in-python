import argparse

import matrix as Matrix

def main(options):
    if options.subcommand == 'make':
        make(options)

def make(options):
    a = Matrix.create_random_diagonal_dominate(options.size)
    soln = Matrix.create_random(options.size, True)
    b = Matrix.multiply(a, soln)

    tdir = Matrix.write_files(
        (a, 'A.def'),
        (b, 'b.def'),
        (soln, 'soln.def'))
    print(tdir)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest='subcommand', help='sub-command help.')

    parser_a = subparsers.add_parser('make', help='Make matrix help')
    parser_a.add_argument('size', type=int, help='Size of matrix A')

    options = parser.parse_args()
    main(options)