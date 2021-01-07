import argparse

from src import make, solve
from src.utils import eprint


def main():
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(
        title="actions", dest="action", help="Action to perform"
    )
    make.add_subparser(subparsers)
    solve.add_subparser(subparsers)
    options = parser.parse_args()

    success = False
    if options.action == "make":
        success = make.main(options)
    elif options.action == "solve":
        success = solve.main(options)

    if not success:
        exit(1)
    exit(0)


if __name__ == "__main__":
    main()
