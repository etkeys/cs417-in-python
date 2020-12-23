import numpy as np
import unittest

from matrix import Matrix

from .utils import *

class TestMatrixCreate(unittest.TestCase):

    def _run(self, **kwargs):
        data = load_test_data(kwargs['file'])
        for test in data:
            with self.subTest('Test: %s' % test.name):
                if isinstance(test.expect, str) and test.expect == 'throws':
                    if 'expect_throws' in kwargs:
                        kwargs['expect_throws'](test)
                    else:
                        self.fail("Condition not implemented!")
                else:
                    kwargs['expect_else'](test)

    def test_create_random(self):
        def expect_throws(test):
            self.assertRaises(
                ValueError,
                Matrix.create_random,
                test.input.size,
                test.input.single_column)
        
        def expect_else(test):
            act = Matrix.create_random(test.input.size, test.input.single_column)
            self.assertEqual(act.shape, (test.expect.rows, test.expect.columns))

        self._run(
            file='matrix.create_random',
            expect_throws=expect_throws,
            expect_else=expect_else
        )

        
    def test_create_random_validate_values(self):
        def expect_else(test):
            act = Matrix.create_random(test.input.size, test.input.single_column)
            self.assertEqual(act.dtype, np.dtype(np.float64))

            mask = np.logical_and(act <= test.expect.upper, act >= test.expect.lower)
            if test.expect.any:
                self.assertTrue(not np.all(mask), 'All values are in range.')
            else:
                self.assertTrue(np.all(mask), 'One value is out of range.')

        self._run(
            file='matrix.create_random_validate_values',
            expect_else=expect_else
        )


    def test_create_random_diag_dominate(self):
        def expect_else(test):
            act = Matrix.create_random_diagonal_dominate(test.input.size)
            self.assertEqual(act.shape, (test.expect.size, test.expect.size))

            diag = act.diagonal()
            bigs = act.max(0)
            self.assertTrue(np.equal(diag, bigs).all())

        self._run(
            file='matrix.create_random_diag_dominate',
            expect_else=expect_else
        )

class TestMatrix(unittest.TestCase):
    def _run(self, **kwargs):
        data = load_test_data(kwargs['file'])
        for test in data:
            with self.subTest('Test: %s' % test.name):
                if isinstance(test.expect, str) and test.expect == 'throws':
                    if 'expect_throws' in kwargs:
                        kwargs['expect_throws'](test)
                    else:
                        self.fail("Condition not implemented!")
                else:
                    kwargs['expect_else'](test)

    def test_multiply(self):
        def expect_else(test):
            a = np.array(test.input.a)
            b = np.array(test.input.b)
            act = Matrix.multiply(a, b)
            exp = np.array(test.expect)
            self.assertTrue(np.equal(act, exp).all())

        self._run(
            file='matrix.multiply',
            expect_else=expect_else
        )


if __name__ == '__main__':
    unittest.main()