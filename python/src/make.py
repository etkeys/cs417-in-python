
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
    parser.add_argument(
        '-d', '--directory',
        default=None,
        dest='directory',
        metavar='DIR',
        type=str,
        help='Directory to output to. If not absolute will be placed in tmp.')

def main(options):
    a = matops.create_random_diagonal_dominate(options.size)
    soln = matops.create_random(options.size, True)
    b = matops.multiply(a, soln)

    tdir = matops.write_files(
        (a, 'A.def'),
        (b, 'b.def'),
        (soln, 'soln.def'),
        directory=options.directory)
    print(tdir)
    return True