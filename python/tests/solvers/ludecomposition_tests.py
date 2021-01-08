import traceback

import numpy as np

import src.matrix_operations as matops
from src.solvers import LuDecompositionSolver
import tests.utils as utils


class TestLuDecomposition(utils.TestCaseBase):
    @property
    def root_test_data_path(self):
        return ["solvers", "ludecomposition"]

    def test_calculate_forward_solve_vector(self):
        def expect_else(test):
            inp_matb = np.array(test.input.matb, dtype=float)
            inp_matL = np.array(test.input.matL, dtype=float)
            inp_matA = np.zeros((inp_matb.size, inp_matb.size))
            exp = np.array(test.expect, dtype=float)

            actor = LuDecompositionSolver(inp_matA, inp_matb)
            act = actor._calculate_forward_solve_vector(inp_matL)

            self.assertTrue(np.allclose(act, exp))

        utils.run_test(self, file="calc_forward_solve_vector", expect_else=expect_else)

    def test_calculate_l_column(self):
        def expect_else(test):
            inp_matA = np.array(test.input.matA, dtype=float)
            inp_matL = np.array(test.input.matL, dtype=float)
            inp_matU = np.array(test.input.matU, dtype=float)
            inp_column = test.input.column
            inp_limit = test.input.limit
            inp_matb = np.ones(inp_limit, dtype=float)
            exp = np.array(test.expect, dtype=float)

            # print('')
            # print(test.name)
            # print(inp_matA)
            # print(inp_matL)
            # print(inp_matU)

            actor = LuDecompositionSolver(inp_matA, inp_matb)
            act = actor._calculate_l_column(inp_matL, inp_matU, inp_column, inp_limit)

            # print("Expected:\n{}".format(exp))
            # print("Actual:\n{}".format(act))
            self.assertTrue(matops.almost_equal(act, exp))

        utils.run_test(self, file="calc_l_column", expect_else=expect_else)

    def test_calculate_u_row(self):
        def expect_else(test):
            inp_matA = np.array(test.input.matA, dtype=float)
            inp_matL = np.array(test.input.matL, dtype=float)
            inp_matU = np.array(test.input.matU, dtype=float)
            inp_column = test.input.row
            inp_limit = test.input.limit
            inp_matb = np.ones(inp_limit, dtype=float)
            exp = np.array(test.expect, dtype=float)

            # print('')
            # print(inp_matA)
            # print(inp_matL)
            # print(inp_matU)

            actor = LuDecompositionSolver(inp_matA, inp_matb)
            act = actor._calculate_u_row(inp_matL, inp_matU, inp_column, inp_limit)

            # print("Expected:\n{}".format(exp))
            # print("Actual:\n{}".format(act))
            self.assertTrue(matops.almost_equal(act, exp))

        utils.run_test(self, file="calc_u_row", expect_else=expect_else)

    def test_calculate_y_vector_value(self):
        def expect_else(test):
            # print('')
            # print(test.name)
            inp_matb = np.array(test.input.matb, dtype=float)
            inp_matL = np.array(test.input.matL, dtype=float)
            inp_vecy = np.array(test.input.vecy, dtype=float)
            inp_index = test.input.index
            inp_matA = np.zeros((inp_matb.size, inp_matb.size))
            exp = test.expect

            actor = LuDecompositionSolver(inp_matA, inp_matb)
            act = actor._calculate_y_vector_value(inp_matL, inp_vecy, inp_index)

            self.assertAlmostEqual(act, exp)

        utils.run_test(self, file="calc_y_vector_value", expect_else=expect_else)

    def test_solve(self):
        def expect_else(test):
            inp_matA = np.array(test.input.matA, dtype=float)
            inp_matb = np.array(test.input.matb, dtype=float)
            exp_fresult = test.expect.func_result
            exp_vresult = np.array(test.expect.vec_result, dtype=float)

            actor = LuDecompositionSolver(inp_matA, inp_matb)
            act_fresult = actor.solve()
            act_vresult = actor.result

            # print(exp_vresult)
            # print(act_vresult)
            # if isinstance(act_vresult, Exception):
            #     traceback.print_tb(act_vresult.__traceback__)

            self.assertTrue(np.allclose(act_vresult, exp_vresult))
            self.assertEqual(act_fresult, exp_fresult)

        utils.run_test(self, file="solve", expect_else=expect_else)
