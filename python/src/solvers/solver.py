import collections
from enum import Enum


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
