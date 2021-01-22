from math import isclose

import numpy as np

from src.solvers import GaussianSolver


def test_calc_back_solve_vector_row(name, data, exception):
    # print('')
    # print(data.name)
    inp_matA = np.array(data.input.matA)
    inp_matb = np.array(data.input.matb)
    inp_vec = np.array(data.input.bsvec)
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
    inp_matA = np.array(data.input.matA)
    inp_matb = np.array(data.input.matb)

    with exception:
        actor = GaussianSolver(inp_matA, inp_matb)
        act = actor._calculate_back_solve_vector()

        exp = np.array(data.expect)

        # print(exp)
        # print(act)
        assert np.allclose(act, exp)


def test_solve(name, data, exception):
    # print('')
    # print(data.name)
    # TODO create utils function to call np.array() passing dtype=float
    # TODO can the creation of inp_matA, inp_matb, actor be converted to a resuable function?
    inp_matA = np.array(data.input.matA)
    inp_matb = np.array(data.input.matb)

    with exception:
        actor = GaussianSolver(inp_matA, inp_matb)
        act_fresult = actor.solve()
        act_vresult = actor.result

        exp_fresult = data.expect.func_result
        exp_vresult = np.array(data.expect.vec_result)

        # print(exp_fresult)
        # print(exp_vresult)
        # print(act_fresult)
        # print(act_vresult)

        assert act_fresult == exp_fresult
        assert np.allclose(act_vresult, exp_vresult)
