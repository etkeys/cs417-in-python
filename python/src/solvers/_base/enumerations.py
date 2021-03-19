from enum import Enum


class IterativeInitialGuess(Enum):
    DEFAULT = 1
    MATRIX_OF_ZEROS = 2
    MATRIX_C = 3
    RANDOM_MATRIX = 4

    @staticmethod
    def as_strings():
        return IterativeInitialGuess._get_mapping_from_string().keys()

    @staticmethod
    def from_string(val: str):
        mapping = IterativeInitialGuess._get_mapping_from_string()
        target = val.lower()
        if target not in mapping:
            raise KeyError("%s not in IterativeInitialGuess mapping" % val)
        return mapping[target]

    @staticmethod
    def to_string(val):
        if not isinstance(val, IterativeInitialGuess):
            raise TypeError(
                "val argument is not of type %s" % type(IterativeInitialGuess)
            )
        return IterativeInitialGuess._get_mapping_to_string()[val]

    @staticmethod
    def _get_mapping_from_string():
        return {
            "default": IterativeInitialGuess.DEFAULT,
            "matrix-c": IterativeInitialGuess.MATRIX_C,
            "random": IterativeInitialGuess.RANDOM_MATRIX,
            "zero": IterativeInitialGuess.MATRIX_OF_ZEROS,
        }

    @staticmethod
    def _get_mapping_to_string():
        return dict(
            reversed(item)
            for item in IterativeInitialGuess._get_mapping_from_string().items()
        )


class ResultAttributes(Enum):
    RESULT_VECTOR = 1
    ITERATIONS = 2
    ERROR = 3
    EIGEN = 4
