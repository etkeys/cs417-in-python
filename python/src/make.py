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
    inputs = matops.load_files(directory, False, True)

    size = int(inputs["matA"][0])
    inputs["matA"] = matops.reshape(inputs["matA"][2:], (size, -1))
    inputs["matb"] = matops.reshape(inputs["matb"][2:], (-1, 1))
    inputs["matsoln"] = matops.reshape(inputs["matsoln"][2:], (-1, 1))
    if "omega" in inputs:
        inputs["omega"] = matops.reshape(inputs["omega"][2:], (-1, 1))

    return inputs


def main(options):
    # print(options)
    if getattr(options, "legacy_dir") is not None:
        inputs = _convert_legacy_files(options.legacy_dir)
    else:
        inputs = {}
        inputs["matA"] = matops.create_random_diagonal_dominate(options.size)
        inputs["matsoln"] = matops.create_random(options.size, True)
        inputs["matb"] = matops.multiply(inputs["matA"], inputs["matsoln"])

    tdir = matops.write_files(inputs, directory=options.directory)

    print(tdir)
    return True
