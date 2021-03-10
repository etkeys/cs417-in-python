from math import isclose

import numpy as np
import pytest

import src.matrix_operations as matops
from src.solvers import SORSolver, IterativeInitialGuess

from src.solvers.solver import ComplexResult, _IterativeSolver
from tests.utils import create_matrix
from . import assert_iterations_count


def test_init(name, data, exception):
    inp_guess = IterativeInitialGuess.from_string(data.input.guess)
    inp_matA = create_matrix(data.input.matA)
    inp_matb = create_matrix(data.input.matb)
    inp_omega = data.input.omega

    with exception:
        assert inp_guess is not None
        actor = SORSolver(inp_matA, inp_matb, inp_guess, inp_omega)
        act_guess = actor._guess_source
        act_matA = actor._matA
        act_matb = actor._matb
        act_omega = actor._omega

        exp_guess = IterativeInitialGuess.from_string(data.expect.guess)
        exp_matA = create_matrix(data.expect.matA)
        exp_matb = matops.reshape(create_matrix(data.expect.matb), (-1, 1))
        exp_omega = data.expect.omega

        assert exp_guess is not None
        assert act_guess == exp_guess
        assert matops.almost_equal(act_matA, exp_matA)
        assert matops.almost_equal(act_matb, exp_matb)
        assert isclose(act_omega, exp_omega)


def test_build_interim_matricies(name, data, exception):
    inp_guess = IterativeInitialGuess.from_string(data.input.guess)
    inp_matA = create_matrix(data.input.matA)
    inp_matb = create_matrix(data.input.matb)

    with exception:
        actor = SORSolver(inp_matA, inp_matb, inp_guess, 0.5)
        actor._build_interim_matricies()

        act_matD = actor._matD
        act_matL = actor._matL
        act_matU = actor._matU

        exp_matD = create_matrix(data.expect.matD)
        exp_matL = create_matrix(data.expect.matL)
        exp_matU = create_matrix(data.expect.matU)

        assert matops.almost_equal(act_matD, exp_matD)
        assert matops.almost_equal(act_matL, exp_matL)
        assert matops.almost_equal(act_matU, exp_matU)


def test_calc_iteration_result(name, data, exception):
    inp_guess = IterativeInitialGuess.from_string(data.input.guess)
    inp_matA = create_matrix(data.input.matA)
    inp_matb = create_matrix(data.input.matb)
    inp_omega = data.input.omega

    with exception:
        actor = SORSolver(inp_matA, inp_matb, inp_guess, inp_omega)
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
        actor = SORSolver(inp_matA, inp_matb, inp_guess, 0.5)
        if not inp_skip_build_interim:
            actor._build_interim_matricies()
        act = actor._create_guess()

        if inp_guess != IterativeInitialGuess.RANDOM_MATRIX:
            exp = matops.reshape(create_matrix(data.expect.mat), (-1, 1))
            assert act.shape == exp.shape
            assert matops.almost_equal(act, exp)
        else:
            act = np.fabs(act)
            exp_shape = actor._matb.shape
            exp_mat_zero = matops.create_zeros(exp_shape[0], exp_shape[1])
            assert act.shape == exp_shape
            assert not matops.almost_equal(act, exp_mat_zero)
            assert (act <= 20.0).all()


def test_iterate_until_solved(name, data, exception):
    inp_guess = IterativeInitialGuess.from_string(data.input.guess)
    inp_matA = create_matrix(data.input.matA)
    inp_matb = create_matrix(data.input.matb)

    with exception:
        actor = SORSolver(inp_matA, inp_matb, inp_guess, 0.5)
        init_guess = actor._create_guess()
        act_vec, act_iter_count = actor._iterate_until_solved(init_guess)
        print(act_vec)

        exp_iter_count = data.expect.iter_count
        print(act_iter_count)
        if isinstance(exp_iter_count, int):
            assert act_iter_count == exp_iter_count
        elif isinstance(exp_iter_count, list):
            assert exp_iter_count[0] <= act_iter_count <= exp_iter_count[1]
        else:
            pytest.fail("expect.iter_count should be int or list of ints.")

        if (
            isinstance(exp_iter_count, list)
            or exp_iter_count < _IterativeSolver._ITER_MAX
        ):
            exp_vec = matops.reshape(create_matrix(data.expect.mat), (-1, 1))
            assert matops.almost_equal(act_vec, exp_vec)


def test_solve(name, data, exception):
    inp_guess = IterativeInitialGuess.from_string(data.input.guess)
    inp_matA = create_matrix(data.input.matA)
    inp_matb = create_matrix(data.input.matb)

    with exception:
        actor = SORSolver(inp_matA, inp_matb, inp_guess, 0.5)
        act_fresult = actor.solve()
        act_result = actor.result

        exp_fresult = data.expect.func_result

        assert act_fresult == exp_fresult
        assert isinstance(act_result, ComplexResult)

        if act_result.has_error:
            raise act_result[ComplexResult.Attributes.ERROR]
        else:
            exp_vec = matops.reshape(create_matrix(data.expect.vec_result), (-1, 1))
            exp_iter_count = data.expect.iter_count

            assert all(
                [
                    att in act_result
                    for att in [
                        ComplexResult.Attributes.RESULT_VECTOR,
                        ComplexResult.Attributes.ITERATIONS,
                    ]
                ]
            )

            assert matops.almost_equal(
                act_result[ComplexResult.Attributes.RESULT_VECTOR], exp_vec
            )
            assert_iterations_count(
                act_result[ComplexResult.Attributes.ITERATIONS], exp_iter_count
            )
