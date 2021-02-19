import collections
from enum import Enum
from math import isclose

import src.matrix_operations as matops


# Based on custom dict from
# https://github.com/wavetossed/mcdict/blob/master/mcdict.py
class ComplexResult(collections.MutableMapping):
    class Attributes(Enum):
        RESULT_VECTOR = 1
        CAVEATS = 2
        ITERATIONS = 3
        ERROR = 4

    class Caveats(Enum):
        NORMAL = 0
        # TODO lame convergence should be "Divergent" and raise exception
        LAME_CONVERGANCE = 1
        ERROR = 200
        MULTIPLE = 255

    _KT = Attributes
    _RC = Caveats

    def __init__(self):
        self._data = {}
        self[self._KT.CAVEATS] = self._RC.NORMAL

    def __bool__(self):
        return self[self._KT.CAVEATS] == self._RC.NORMAL

    def __delitem__(self, k: _KT):
        if k in self:
            self._data.delete[k]

    def __contains__(self, k: _KT):
        return k in self._data

    def __getitem__(self, k: _KT):
        ret = self._data.get(k, None)
        return ret

    def __iter__(self):
        keys = set(self._data.keys())
        # The attributes that must appear first and in this order!!!
        first = [
            self._KT.RESULT_VECTOR,
            self._KT.CAVEATS,
            self._KT.ERROR,
        ]
        ikeys = []
        for k in first:
            if k in keys:
                ikeys.append(k)
                keys.remove(k)
        ikeys.extend(keys)
        self._iter_keys = ikeys
        self._iter_curr = -1  # need to increment this before accessing
        self._len = (
            len(ikeys) - 1
        )  # account for strange increment style, whatever that is
        return self

    def __len__(self):
        return len(self._data)

    def __next__(self):
        return self.next()

    def __repr__(self):
        return repr(self._data)

    def __setitem__(self, k: _KT, v) -> None:
        if k == self._KT.CAVEATS:
            self._add_caveat(v)
        else:
            self._data[k] = v

    def next(self):
        if self._iter_curr < self._len:
            self._iter_curr += 1
            ret_key = self._iter_keys[self._iter_curr]
            return self._data[ret_key]
        raise StopIteration

    def has_error(self):
        return self.Caveats.ERROR in self._data[self._KT.CAVEATS]

    def _add_caveat(self, caveat: _RC):
        data = self._data
        key = self._KT.CAVEATS
        if caveat == self._RC.NORMAL:
            data[key] = [caveat]
            return

        if data[key][0] == self._RC.NORMAL:
            data[key] = []
        elif data[key][0] != self._RC.MULTIPLE:
            data[key] = [self._RC.MULTIPLE] + data[key]

        data[key].append(caveat)


class _Solver(object):
    @staticmethod
    def get_solver_name():
        raise NotImplementedError("_Solver does not implement get_solver_name().")

    def solve(self):
        raise NotImplementedError("_Solver does not implement solve().")

    @property
    def result(self):
        if self._result is None:
            raise RuntimeError("No result is available for this solver.")
        return self._result


class _ComplexResultSolver(_Solver):
    def __init__(self):
        self._result = ComplexResult()


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


class _IterativeSolver(_ComplexResultSolver):
    _ITER_MAX = 1000

    _allowed_guess_sources = [
        IterativeInitialGuess.DEFAULT,
        IterativeInitialGuess.MATRIX_C,
        IterativeInitialGuess.MATRIX_OF_ZEROS,
        IterativeInitialGuess.RANDOM_MATRIX,
    ]

    def __init__(self, matA, matb, guess_source: IterativeInitialGuess):
        super().__init__()
        if not matops.is_matrix(matA):
            raise ValueError("Matrix A missing or not a matrix.")
        if not matops.is_square(matA):
            raise ValueError("Matrix A is not square.")
        if not matops.is_vector(matb):
            raise ValueError("Matrix b missing or not a vector.")
        if guess_source not in self._allowed_guess_sources:
            raise ValueError(
                "Guess choice {} is not valid in this context.".format(guess_source)
            )

        self._guess_source = guess_source
        self._matA = matops.deepcopy(matA)
        self._matb = matops.deepcopy(
            matb if matops.is_vvector(matb) else matops.reshape(matb, (-1, 1))
        )

    def solve(self):
        _RA = ComplexResult.Attributes
        try:
            self._build_interim_matricies()
            guess = self._create_guess()

            res, iter_count = self._iterate_until_solved(guess)
            self._result[_RA.RESULT_VECTOR] = res
            self._result[_RA.ITERATIONS] = iter_count

            if iter_count > self._ITER_MAX:
                self._result[_RA.CAVEATS] = ComplexResult.Caveats.LAME_CONVERGANCE
        except Exception as ex:
            self._result[_RA.ERROR] = ex
            self._result[_RA.CAVEATS] = ComplexResult.Caveats.ERROR
            return False
        return True

    def _build_interim_matricies(self):
        raise NotImplementedError(
            "_IterativeSolver does not implement _build_interim_matricies()"
        )

    def _calculate_iteration_result(self, mat_guess):
        raise NotImplementedError(
            "_IterativeSolver does not implement _calculate_iteration_result()"
        )

    def _create_guess(self):
        raise NotImplementedError("_IterativeSolver does not implement _create_guess()")

    def _iterate_until_solved(self, mat_guess):
        two_norm = 1.0
        iter_count = 0
        ret = matops.deepcopy(mat_guess)
        while self._should_iteration_continue(iter_count, two_norm):
            ret = self._calculate_iteration_result(ret)
            two_norm = matops.two_norm_of_error(self._matA, self._matb, ret)
            iter_count += 1

        return (ret, iter_count)

    def _should_iteration_continue(self, iter_count, two_norm):
        tol = 0.000_05
        if isclose(two_norm, 0.0, abs_tol=tol):
            return False
        elif iter_count > self._ITER_MAX:
            return False
        else:
            return True
