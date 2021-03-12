from enum import IntEnum


class ExitCodes(IntEnum):
    SUCCESS = 0
    GENERAL_ERROR = 1
    BAD_CLI = 2
    DIVERGENT_SOLUTION = 3
    SOLUTION_INCORRECT = 4
    UNEXPECTED_ERROR = 127
