
import src.matrix_operations as matops

def add_subparser(subparsers):
    parser = subparsers.add_parser(
        'make',
        description='Create matrix definition files suitable for use with solve',
        help='Create matrix definition files'
    )
    parser.add_argument(
        'size',
        metavar='SIZE',
        type=int,
        help='Size of matrix A')

def main(options):
    a = matops.create_random_diagonal_dominate(options.size)
    soln = matops.create_random(options.size, True)
    b = matops.multiply(a, soln)

    tdir = matops.write_files(
        (a, 'A.def'),
        (b, 'b.def'),
        (soln, 'soln.def'))
    print(tdir)
    return True