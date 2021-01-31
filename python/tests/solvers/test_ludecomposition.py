from math import isclose
import traceback

import numpy as np

import src.matrix_operations as matops
from src.solvers import LuDecompositionSolver
from tests.utils import create_matrix


def test_calc_forward_solve_vector(name, data, exception):
    inp_matb = create_matrix(data.input.matb)
    inp_matL = create_matrix(data.input.matL)
    inp_matA = matops.create_zeros(inp_matb.size, inp_matb.size)

    with exception:
        actor = LuDecompositionSolver(inp_matA, inp_matb)
        act = actor._calculate_forward_solve_vector(inp_matL)

        exp = create_matrix(data.expect)

        assert matops.almost_equal(act, exp)


def test_calc_l_column(name, data, exception):
    inp_matA = create_matrix(data.input.matA)
    inp_matL = create_matrix(data.input.matL)
    inp_matU = create_matrix(data.input.matU)
    inp_column = data.input.column
    inp_limit = data.input.limit
    inp_matb = np.ones(inp_limit)

    with exception:
        # print('')
        # print(test.name)
        # print(inp_matA)
        # print(inp_matL)
        # print(inp_matU)
        actor = LuDecompositionSolver(inp_matA, inp_matb)
        act = actor._calculate_l_column(inp_matL, inp_matU, inp_column, inp_limit)

        exp = create_matrix(data.expect)

        # print("Expected:\n{}".format(exp))
        # print("Actual:\n{}".format(act))
        assert matops.almost_equal(act, exp)


def test_calc_u_row(name, data, exception):
    inp_matA = create_matrix(data.input.matA)
    inp_matL = create_matrix(data.input.matL)
    inp_matU = create_matrix(data.input.matU)
    inp_column = data.input.row
    inp_limit = data.input.limit
    inp_matb = np.ones(inp_limit)

    with exception:
        # print('')
        # print(inp_matA)
        # print(inp_matL)
        # print(inp_matU)
        actor = LuDecompositionSolver(inp_matA, inp_matb)
        act = actor._calculate_u_row(inp_matL, inp_matU, inp_column, inp_limit)

        exp = create_matrix(data.expect)

        # print("Expected:\n{}".format(exp))
        # print("Actual:\n{}".format(act))
        assert matops.almost_equal(act, exp)


def test_calc_y_vector_value(name, data, exception):
    # print('')
    # print(data.name)
    inp_matb = create_matrix(data.input.matb)
    inp_matL = create_matrix(data.input.matL)
    inp_vecy = create_matrix(data.input.vecy)
    inp_index = data.input.index
    inp_matA = matops.create_zeros(inp_matb.size, inp_matb.size)

    with exception:
        actor = LuDecompositionSolver(inp_matA, inp_matb)
        act = actor._calculate_y_vector_value(inp_matL, inp_vecy, inp_index)

        exp = data.expect

        assert isclose(act, exp)


def test_solve(name, data, exception):
    inp_matA = create_matrix(data.input.matA)
    inp_matb = create_matrix(data.input.matb)

    with exception:
        actor = LuDecompositionSolver(inp_matA, inp_matb)
        act_fresult = actor.solve()
        act_vresult = actor.result

        exp_fresult = data.expect.func_result
        exp_vresult = create_matrix(data.expect.vec_result)

        # print(exp_vresult)
        # print(act_vresult)
        # if isinstance(act_vresult, Exception):
        #     traceback.print_tb(act_vresult.__traceback__)

        assert act_fresult == exp_fresult
        assert matops.almost_equal(act_vresult, exp_vresult)
