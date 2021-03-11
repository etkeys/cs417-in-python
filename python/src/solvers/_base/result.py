from collections import MutableMapping

from .enumerations import ResultAttributes

# Based on custom dict from
# https://github.com/wavetossed/mcdict/blob/master/mcdict.py
class Result(MutableMapping):
    def __init__(self):
        self._data = {}

    def __bool__(self):
        return not self.has_error()

    def __delitem__(self, k: ResultAttributes):
        if k in self:
            self._data.delete[k]

    def __contains__(self, k: ResultAttributes):
        return k in self._data

    def __getitem__(self, k: ResultAttributes):
        ret = self._data.get(k, None)
        return ret

    def __iter__(self):
        keys = set(self._data.keys())
        # The attributes that must appear first and in this order!!!
        first = [
            ResultAttributes.RESULT_VECTOR,
            ResultAttributes.ERROR,
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

    def __setitem__(self, k: ResultAttributes, v) -> None:
        self._data[k] = v

    def next(self):
        if self._iter_curr < self._len:
            self._iter_curr += 1
            ret_key = self._iter_keys[self._iter_curr]
            return self._data[ret_key]
        raise StopIteration

    @property
    def has_error(self):
        return self._data.get(ResultAttributes.ERROR, None) is not None
