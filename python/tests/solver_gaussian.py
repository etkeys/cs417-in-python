
import numpy as np
import unittest

import matrix as Matrix
from solvers import GaussianSolver

from .utils import *

class TestGaussian(unittest.TestCase):

    def test_calc_back_solve_vector_row(self):
        def expect_else(data):
            # print('')
            # print(data.name)
            inp_mat = np.array(data.input.mat)
            inp_vec = np.array(data.input.bsvec)
            inp_row = data.input.calc_row
            exp = data.expect
            actor = GaussianSolver(inp_mat)
            act = actor._calculate_back_solve_vector_row(inp_vec, inp_row)

            # print(exp)
            # print(act)
            self.assertAlmostEqual(act, exp, 5)

        run_test(
            self,
            file='gaussian.calc_back_solve_vector_row',
            expect_else=expect_else
        )

    def test_calc_back_solve_vector(self):
        def expect_else(data):
            # print('')
            # print(data.name)
            inp = np.array(data.input)
            exp = np.array(data.expect)
            actor = GaussianSolver(inp)
            act = actor._calculate_back_solve_vector()

            # print(exp)
            # print(act)
            self.assertTrue(np.allclose(act, exp))

        run_test(
            self,
            file='gaussian.calc_back_solve_vector',
            expect_else=expect_else
        )

    def test_solve(self):
        def expect_else(data):
            # print('')
            # print(data.name)
            inp = np.array(data.input)
            exp_fresult = data.expect.func_result
            exp_vresult = data.expect.vec_result
            actor = GaussianSolver(inp)
            act_fresult = actor.solve()
            act_vresult = actor.result


            # print(exp_fresult)
            # print(exp_vresult)
            # print(act_fresult)
            # print(act_vresult)

            self.assertEqual(act_fresult, exp_fresult)
            self.assertTrue(np.allclose(act_vresult, exp_vresult))

        run_test(
            self,
            file='gaussian.solve',
            expect_else=expect_else
        )

