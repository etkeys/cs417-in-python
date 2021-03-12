import argparse
import traceback

from src import make, solve
import src.exceptions as app_errors
from src.exit_codes import ExitCodes
from src.utils import eprint


def main():
    try:
        parser = argparse.ArgumentParser()
        subparsers = parser.add_subparsers(
            title="actions", dest="action", help="Action to perform"
        )
        make.add_subparser(subparsers)
        solve.add_subparser(subparsers)
        options = parser.parse_args()

        if options.action == "make":
            make.main(options)
        elif options.action == "solve":
            solve.main(options)

        exit(ExitCodes.SUCCESS)
    except argparse.ArgumentError as e:
        eprint(e)
        exit(ExitCodes.BAD_CLI)
    except app_errors.DivergentSolution as e:
        eprint(e)
        eprint(traceback.print_tb(e.__traceback__))
        exit(ExitCodes.DIVERGENT_SOLUTION)
    except FileNotFoundError as e:
        eprint(e)
        exit(ExitCodes.GENERAL_ERROR)
    except app_errors.SolutionValidation as e:
        eprint(e)
        exit(ExitCodes.SOLUTION_INCORRECT)
    except TypeError as e:
        eprint(e)
        exit(ExitCodes.GENERAL_ERROR)
    except ValueError as e:
        eprint(e)
        exit(ExitCodes.GENERAL_ERROR)
    except ZeroDivisionError as e:
        eprint(e)
        exit(ExitCodes.GENERAL_ERROR)
    except Exception as e:
        eprint("%s: %s" % (type(e).__name__, e))
        exit(ExitCodes.UNEXPECTED_ERROR)


if __name__ == "__main__":
    main()
