from math import isclose

from src.matrix_operations import almost_equal
from src.solvers import GaussianSolver
from tests.utils import create_matrix


def test_calc_back_solve_vector_row(name, data, exception):
    # print('')
    # print(data.name)
    inp_matA = create_matrix(data.input.matA)
    inp_matb = create_matrix(data.input.matb)
    inp_vec = create_matrix(data.input.bsvec)
    inp_row = data.input.calc_row

    with exception:
        actor = GaussianSolver(inp_matA, inp_matb)
        act = actor._calculate_back_solve_vector_row(inp_vec, inp_row)

        exp = data.expect

        # print(exp)
        # print(act)
        assert isclose(act, exp, abs_tol=0.000_01)


def test_calc_back_solve_vector(name, data, exception):
    # print('')
    # print(data.name)
    inp_matA = create_matrix(data.input.matA)
    inp_matb = create_matrix(data.input.matb)

    with exception:
        actor = GaussianSolver(inp_matA, inp_matb)
        act = actor._calculate_back_solve_vector()

        exp = create_matrix(data.expect)

        # print(exp)
        # print(act)
        assert almost_equal(act, exp)


def test_solve(name, data, exception):
    # print('')
    # print(data.name)
    inp_matA = create_matrix(data.input.matA)
    inp_matb = create_matrix(data.input.matb)

    with exception:
        actor = GaussianSolver(inp_matA, inp_matb)
        act_fresult = actor.solve()
        act_vresult = actor.result

        exp_fresult = data.expect.func_result
        exp_vresult = create_matrix(data.expect.vec_result)

        # print(exp_fresult)
        # print(exp_vresult)
        # print(act_fresult)
        # print(act_vresult)

        assert act_fresult == exp_fresult
        assert almost_equal(act_vresult, exp_vresult)
