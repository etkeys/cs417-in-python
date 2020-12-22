import argparse

from matrix import Matrix

def make(options):
    if options.size < 2 or options.size > 500:
        raise ValueError('size input must be between 2..500, here is %i' % options.size)

    a = Matrix.create_random_diagonal_dominate(options.size)
    soln = Matrix.create_random(options.size, True)
    b = Matrix.multiply(a, soln)

    tdir = Matrix.write_files(
        (a, 'A.def'),
        (b, 'b.def'),
        (soln, 'soln.def'))
    print(tdir)

def main(options):
    if options.subcommand == 'make':
        make(options)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest='subcommand', help='sub-command help.')

    parser_a = subparsers.add_parser('make', help='Make matrix help')
    parser_a.add_argument('size', type=int, help='Size of matrix A')

    options = parser.parse_args()
    main(options)