import src.matrix_operations as matops


def add_subparser(subparsers):
    parser = subparsers.add_parser(
        "make",
        description="Create matrix definition files suitable for use with solve",
        help="Create matrix definition files",
    )
    exgroup = parser.add_mutually_exclusive_group()
    exgroup.add_argument(
        "size", metavar="SIZE", type=int, nargs="?", help="Size of matrix A"
    )
    exgroup.add_argument(
        "--from-legacy",
        dest="legacy_dir",
        metavar="LEG_DIR",
        type=str,
        help="Convert legacy files in directory LEG_DIR to updated format.",
    )
    parser.add_argument(
        "-d",
        "--directory",
        default=None,
        dest="directory",
        metavar="DIR",
        type=str,
        help="Directory to output to. If not absolute will be placed in tmp.",
    )


def _convert_legacy_files(directory):
    matA, matb, matsoln = matops.load_files(directory, False)

    size = int(matA[0])
    matA = matops.reshape(matA[2:], (size, -1))
    matb = matops.reshape(matb[2:], (-1, 1))
    matsoln = matops.reshape(matsoln[2:], (-1, 1))

    return (matA, matb, matsoln)


def main(options):
    # print(options)
    if getattr(options, "legacy_dir") is not None:
        matA, matb, matsoln = _convert_legacy_files(options.legacy_dir)
    else:
        matA = matops.create_random_diagonal_dominate(options.size)
        matsoln = matops.create_random(options.size, True)
        matb = matops.multiply(matA, matsoln)

    tdir = matops.write_files(
        (matA, "A.def"),
        (matb, "b.def"),
        (matsoln, "soln.def"),
        directory=options.directory,
    )
    print(tdir)
    return True
