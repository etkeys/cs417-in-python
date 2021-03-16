from math import isclose

from src.matrix_operations import almost_equal, reshape
from src.solvers import GaussianSolver
from tests.utils import create_matrix
from . import common_test_solve


def test_init(name, data, exception):
    inp_matA = create_matrix(data.input.matA)
    inp_matb = create_matrix(data.input.matb)

    with exception:
        actor = GaussianSolver(matA=inp_matA, matb=inp_matb)
        act_matA = actor._matA
        act_matb = actor._matb
        act_mat = actor._mat

        exp_matA = create_matrix(data.expect.matA)
        exp_matb = create_matrix(data.expect.matb)
        exp_mat = create_matrix(data.expect.mat)

        assert almost_equal(act_matA, exp_matA)
        assert almost_equal(act_matb, exp_matb)
        assert almost_equal(act_mat, exp_mat)


def test_calc_back_solve_vector_row(name, data, exception):
    inp_matA = create_matrix(data.input.matA)
    inp_matb = create_matrix(data.input.matb)
    inp_vec = create_matrix(data.input.bsvec)
    inp_row = data.input.calc_row

    with exception:
        actor = GaussianSolver(matA=inp_matA, matb=inp_matb)
        act = actor._calculate_back_solve_vector_row(inp_vec, inp_row)

        exp = data.expect

        # print(exp)
        # print(act)
        assert isclose(act, exp, abs_tol=0.000_01)


def test_calc_back_solve_vector(name, data, exception):
    inp_matA = create_matrix(data.input.matA)
    inp_matb = create_matrix(data.input.matb)

    with exception:
        actor = GaussianSolver(matA=inp_matA, matb=inp_matb)
        act = actor._calculate_back_solve_vector()

        exp = create_matrix(data.expect)

        # print(exp)
        # print(act)
        assert almost_equal(act, exp)


def test_solve(name, data, exception):
    common_test_solve(data, exception, GaussianSolver)
