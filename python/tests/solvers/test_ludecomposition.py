from math import isclose

import numpy as np

import src.matrix_operations as matops
from src.solvers import LuDecompositionSolver
from tests.utils import create_matrix
from . import common_test_solve

# TODO need to test for __init__


def test_calc_forward_solve_vector(name, data, exception):
    inp_matb = create_matrix(data.input.matb)
    inp_matL = create_matrix(data.input.matL)
    inp_matA = matops.create_zeros(inp_matb.size, inp_matb.size)

    with exception:
        actor = LuDecompositionSolver(matA=inp_matA, matb=inp_matb)
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
        actor = LuDecompositionSolver(matA=inp_matA, matb=inp_matb)
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
        actor = LuDecompositionSolver(matA=inp_matA, matb=inp_matb)
        act = actor._calculate_u_row(inp_matL, inp_matU, inp_column, inp_limit)

        exp = create_matrix(data.expect)

        # print("Expected:\n{}".format(exp))
        # print("Actual:\n{}".format(act))
        assert matops.almost_equal(act, exp)


def test_calc_y_vector_value(name, data, exception):
    inp_matb = create_matrix(data.input.matb)
    inp_matL = create_matrix(data.input.matL)
    inp_vecy = create_matrix(data.input.vecy)
    inp_index = data.input.index
    inp_matA = matops.create_zeros(inp_matb.size, inp_matb.size)

    with exception:
        actor = LuDecompositionSolver(matA=inp_matA, matb=inp_matb)
        act = actor._calculate_y_vector_value(inp_matL, inp_vecy, inp_index)

        exp = data.expect

        assert isclose(act, exp)


def test_solve(name, data, exception):
    common_test_solve(data, exception, LuDecompositionSolver)
