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
        return iter(self._data)

    def __len__(self):
        return len(self._data)

    def __repr__(self):
        return repr(self._data)

    def __setitem__(self, k: ResultAttributes, v) -> None:
        self._data[k] = v

    @property
    def has_error(self):
        return self._data.get(ResultAttributes.ERROR, None) is not None
