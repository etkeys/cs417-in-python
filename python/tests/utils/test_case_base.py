
import unittest

class TestCaseBase(unittest.TestCase):

    @property
    def root_test_data_path(self):
        raise NotImplementedError('TestCaseBase derived class does not implment propety')
