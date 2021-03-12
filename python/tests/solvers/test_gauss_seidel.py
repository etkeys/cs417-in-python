import numpy as np
import pytest

import src.matrix_operations as matops
from src.solvers import GaussSeidelSolver, IterativeInitialGuess

from tests.utils import create_matrix
from . import common_test_solve

_ITER_MAX = GaussSeidelSolver._ITER_MAX


def test_init(name, data, exception):
    inp_guess = IterativeInitialGuess.from_string(data.input.guess)
    inp_matA = create_matrix(data.input.matA)
    inp_matb = create_matrix(data.input.matb)

    with exception:
        assert inp_guess is not None
        actor = GaussSeidelSolver(matA=inp_matA, matb=inp_matb, guess_source=inp_guess)
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
    inp_matb = create_matrix(data.input.matb)

    with exception:
        actor = GaussSeidelSolver(matA=inp_matA, matb=inp_matb, guess_source=inp_guess)
        actor._build_interim_matricies()

        act_matC = actor._matC
        act_matD = actor._matD

        exp_matC = matops.reshape(create_matrix(data.expect.matC), (-1, 1))
        exp_matD = create_matrix(data.expect.matD)

        assert matops.almost_equal(act_matC, exp_matC)
        assert matops.almost_equal(act_matD, exp_matD)


def test_calc_iteration_result(name, data, exception):
    inp_guess = IterativeInitialGuess.from_string(data.input.guess)
    inp_matA = create_matrix(data.input.matA)
    inp_matb = create_matrix(data.input.matb)

    with exception:
        actor = GaussSeidelSolver(matA=inp_matA, matb=inp_matb, guess_source=inp_guess)
        inp_guess_mat = actor._create_guess()

        act = actor._calculate_iteration_result(inp_guess_mat)

        exp = matops.reshape(create_matrix(data.expect.mat), (-1, 1))

        if inp_guess != IterativeInitialGuess.RANDOM_MATRIX:
            assert matops.almost_equal(act, exp)
        else:
            # What is the expectation for random? Just that it didn't fail?
            pass


def test_create_guess(name, data, exception):
    inp_guess = IterativeInitialGuess.from_string(data.input.guess)
    inp_matA = create_matrix(data.input.matA)
    inp_matb = create_matrix(data.input.matb)
    inp_skip_build_interim = data.input.skip_build_interim

    with exception:
        actor = GaussSeidelSolver(matA=inp_matA, matb=inp_matb, guess_source=inp_guess)
        if not inp_skip_build_interim:
            actor._build_interim_matricies()
        act = actor._create_guess()

        if inp_guess != IterativeInitialGuess.RANDOM_MATRIX:
            exp = matops.reshape(create_matrix(data.expect.mat), (-1, 1))
            assert act.shape == exp.shape
            assert matops.almost_equal(act, exp)
        else:
            act = np.fabs(act)
            exp_shape = actor._matC.shape
            exp_mat_zero = matops.create_zeros(exp_shape[0], exp_shape[1])
            assert act.shape == exp_shape
            assert not matops.almost_equal(act, exp_mat_zero)
            assert (act <= 20.0).all()


def test_iterate_until_solved(name, data, exception):
    def assert_result_vector_equality(act_vec, data_exp_raw):
        exp_vec = matops.reshape(create_matrix(data_exp_raw), (-1, 1))
        assert matops.almost_equal(act_vec, exp_vec)

    inp_guess = IterativeInitialGuess.from_string(data.input.guess)
    inp_matA = create_matrix(data.input.matA)
    inp_matb = create_matrix(data.input.matb)

    with exception:
        actor = GaussSeidelSolver(matA=inp_matA, matb=inp_matb, guess_source=inp_guess)
        init_guess = actor._create_guess()
        act_vec, act_iter_count = actor._iterate_until_solved(init_guess)

        exp_iter_count = data.expect.iter_count
        print(act_iter_count)
        if isinstance(exp_iter_count, int):
            assert act_iter_count == exp_iter_count
            if exp_iter_count <= _ITER_MAX:
                assert_result_vector_equality(act_vec, data.expect.mat)
        elif isinstance(exp_iter_count, list):
            assert exp_iter_count[0] <= act_iter_count <= exp_iter_count[1]
            assert_result_vector_equality(act_vec, data.expect.mat)
        else:
            pytest.fail("expect.iter_count should be int or list of ints.")


def test_solve(name, data, exception):
    common_test_solve(data, exception, GaussSeidelSolver)
