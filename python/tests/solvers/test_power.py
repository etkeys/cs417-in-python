import numpy as np
import pytest

import src.matrix_operations as matops
from src.solvers import PowerSolver, IterativeInitialGuess

from tests.utils import create_matrix
from . import common_test_solve


def test_init(name, data, exception):
    inp_guess = IterativeInitialGuess.from_string(data.input.guess)
    inp_matA = create_matrix(data.input.matA)

    with exception:
        assert inp_guess is not None
        actor = PowerSolver(matA=inp_matA, guess_source=inp_guess)
        act_guess = actor._guess_source
        act_matA = actor._matA
        act_matb = actor._matb

        exp_guess = IterativeInitialGuess.from_string(data.expect.guess)
        exp_matA = create_matrix(data.expect.matA)
        exp_matb = matops.reshape(create_matrix(data.expect.matb), (-1, 1))

        assert exp_guess is not None
        assert act_guess == exp_guess
        assert matops.almost_equal(act_matA, exp_matA)
        assert matops.almost_equal(act_matb, exp_matb)


def test_build_interim_matricies(name, data, exception):
    inp_guess = IterativeInitialGuess.from_string(data.input.guess)
    inp_matA = create_matrix(data.input.matA)

    with exception:
        actor = PowerSolver(matA=inp_matA, guess_source=inp_guess)
        actor._build_interim_matricies()

        # Repeat the same validation as init, because _build_interim_matricies
        # should do nothing
        act_guess = actor._guess_source
        act_matA = actor._matA
        act_matb = actor._matb

        exp_guess = IterativeInitialGuess.from_string(data.expect.guess)
        exp_matA = create_matrix(data.expect.matA)
        exp_matb = matops.reshape(create_matrix(data.expect.matb), (-1, 1))

        assert exp_guess is not None
        assert act_guess == exp_guess
        assert matops.almost_equal(act_matA, exp_matA)
        assert matops.almost_equal(act_matb, exp_matb)


# def test_calc_iteration_result(name, data, exception):
#     inp_guess = IterativeInitialGuess.from_string(data.input.guess)
#     inp_matA = create_matrix(data.input.matA)
#     inp_matb = create_matrix(data.input.matb)
#     inp_matC = create_matrix(data.input.matC)
#     inp_matD = create_matrix(data.input.matD)
#     inp_matguess = matops.reshape(create_matrix(data.input.mat_guess), (-1, 1))

#     with exception:
#         actor = JacobiSolver(matA=inp_matA, matb=inp_matb, guess_source=inp_guess)
#         actor._matC = inp_matC
#         actor._matD = inp_matD
#         act = actor._calculate_iteration_result(inp_matguess)

#         exp = matops.reshape(create_matrix(data.expect.mat), (-1, 1))

#         assert matops.almost_equal(act, exp)


# def test_create_guess(name, data, exception):
#     inp_guess = IterativeInitialGuess.from_string(data.input.guess)
#     inp_matA = create_matrix(data.input.matA)
#     inp_matb = create_matrix(data.input.matb)
#     inp_skip_build_interim = data.input.skip_build_interim

#     with exception:
#         actor = JacobiSolver(matA=inp_matA, matb=inp_matb, guess_source=inp_guess)
#         if not inp_skip_build_interim:
#             actor._build_interim_matricies()
#         act = actor._create_guess()

#         if inp_guess != IterativeInitialGuess.RANDOM_MATRIX:
#             exp = matops.reshape(create_matrix(data.expect.mat), (-1, 1))
#             assert act.shape == exp.shape
#             assert matops.almost_equal(act, exp)
#         else:
#             act = np.fabs(act)
#             exp_shape = actor._matb.shape
#             exp_mat_zero = matops.create_zeros(exp_shape[0], exp_shape[1])
#             assert act.shape == exp_shape
#             assert not matops.almost_equal(act, exp_mat_zero)
#             assert (act <= 20.0).all()


# def test_iterate_until_solved(name, data, exception):
#     inp_guess = IterativeInitialGuess.from_string(data.input.guess)
#     inp_matA = create_matrix(data.input.matA)
#     inp_matb = create_matrix(data.input.matb)
#     inp_matC = create_matrix(data.input.matC)
#     inp_matD = create_matrix(data.input.matD)

#     with exception:
#         actor = JacobiSolver(matA=inp_matA, matb=inp_matb, guess_source=inp_guess)
#         actor._matC = inp_matC
#         actor._matD = inp_matD
#         init_guess = actor._create_guess()
#         act_vec, act_iter_count = actor._iterate_until_solved(init_guess)

#         exp_vec = matops.reshape(create_matrix(data.expect.mat), (-1, 1))
#         exp_iter_count = data.expect.iter_count

#         assert matops.almost_equal(act_vec, exp_vec)
#         print(act_iter_count)
#         if isinstance(data.expect.iter_count, int):
#             assert act_iter_count == exp_iter_count
#         elif isinstance(data.expect.iter_count, list):
#             assert exp_iter_count[0] <= act_iter_count <= exp_iter_count[1]
#         else:
#             pytest.fail("expect.iter_count should be int or list of ints.")


# def test_solve(name, data, exception):
#     common_test_solve(data, exception, JacobiSolver)
