
class _Solver(object):
    
    def solve(self):
        raise NotImplementedError('_Solver does not implement solve()')

    @property
    def result(self):
        return self._result