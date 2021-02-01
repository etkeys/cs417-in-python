import pytest

import src.matrix_operations as matops
from src.solvers import JacobiSolver

# from src.solvers.jacobi import InitialGuess
from src.solvers.solver import ComplexResult
from tests import utils
from tests.utils import create_matrix, string_to_type


def initial_guess_path(member):
    return f"src.solvers.jacobi.InitialGuess.{member}"


def caveats_path(member):
    return f"src.solvers.solver.ComplexResult.Caveats.{member}"


def test_init(name, data, exception):
    inp_guess = string_to_type(initial_guess_path(data.input.guess))
    inp_matA = create_matrix(data.input.matA)
    inp_matb = create_matrix(data.input.matb)

    with exception:
        assert inp_guess is not None
        actor = JacobiSolver(inp_matA, inp_matb, inp_guess)
        act_guess = actor._guess_source
        act_matA = actor._matA
        act_matb = actor._matb

        exp_guess = string_to_type(initial_guess_path(data.expect.guess))
        exp_matA = create_matrix(data.expect.matA)
        exp_matb = matops.reshape(create_matrix(data.expect.matb), (-1, 1))

        assert exp_guess is not None
        assert act_guess == exp_guess
        assert matops.almost_equal(act_matA, exp_matA)
        assert matops.almost_equal(act_matb, exp_matb)


def test_calc_iteration_result(name, data, exception):
    inp_guess = string_to_type(initial_guess_path(data.input.guess))
    inp_matA = create_matrix(data.input.matA)
    inp_matb = create_matrix(data.input.matb)
    inp_matC = create_matrix(data.input.matC)
    inp_matD = create_matrix(data.input.matD)
    inp_matguess = matops.reshape(create_matrix(data.input.mat_guess), (-1, 1))

    with exception:
        actor = JacobiSolver(inp_matA, inp_matb, inp_guess)
        actor._matC = inp_matC
        actor._matD = inp_matD
        act = actor._calculate_iteration_result(inp_matguess)

        exp = matops.reshape(create_matrix(data.expect.mat), (-1, 1))

        assert matops.almost_equal(act, exp)


def test_create_interim_matricies(name, data, exception):
    inp_guess = string_to_type(initial_guess_path(data.input.guess))
    inp_matA = create_matrix(data.input.matA)
    inp_matb = create_matrix(data.input.matb)

    with exception:
        actor = JacobiSolver(inp_matA, inp_matb, inp_guess)
        act_matC, act_matD = actor._create_interim_matricies()

        exp_matC = create_matrix(data.expect.matC)
        exp_matD = create_matrix(data.expect.matD)

        assert matops.almost_equal(act_matC, exp_matC)
        assert matops.almost_equal(act_matD, exp_matD)


def test_iterate_until_solved(name, data, exception):
    inp_guess = string_to_type(initial_guess_path(data.input.guess))
    inp_matA = create_matrix(data.input.matA)
    inp_matb = create_matrix(data.input.matb)
    inp_matC = create_matrix(data.input.matC)
    inp_matD = create_matrix(data.input.matD)

    with exception:
        actor = JacobiSolver(inp_matA, inp_matb, inp_guess)
        actor._matC = inp_matC
        actor._matD = inp_matD
        act_vec, act_iter_count = actor._iterate_until_solved()

        exp_vec = matops.reshape(create_matrix(data.expect.mat), (-1, 1))
        exp_iter_count = data.expect.iter_count

        assert matops.almost_equal(act_vec, exp_vec)
        print(act_iter_count)
        if isinstance(data.expect.iter_count, int):
            assert act_iter_count == exp_iter_count
        elif isinstance(data.expect.iter_count, list):
            assert exp_iter_count[0] <= act_iter_count <= exp_iter_count[1]
        else:
            pytest.fail("expect.iter_count should be int or list of ints.")


def test_solve(name, data, exception):
    def assert_result_caveats(act_cavs, raw_exp_cavs):
        exp_cavs = [string_to_type(caveats_path(p)) for p in raw_exp_cavs]

        assert len(act_cavs) == len(exp_cavs)
        for acav, ecav in zip(act_cavs, exp_cavs):
            assert acav == ecav

    def assert_iterations_count(act_count, exp_count):
        if isinstance(exp_count, int):
            assert act_count == exp_count
        elif isinstance(exp_count, list):
            assert exp_count[0] <= act_count <= exp_count[1]
        else:
            pytest.fail("exp_count should be int or list of ints.")

    def assert_result_schema(inp, *attributes):
        assert isinstance(inp, ComplexResult)
        for attr in attributes:
            assert attr in inp

    inp_guess = string_to_type(initial_guess_path(data.input.guess))
    inp_matA = create_matrix(data.input.matA)
    inp_matb = create_matrix(data.input.matb)

    with exception:
        actor = JacobiSolver(inp_matA, inp_matb, inp_guess)
        act_fresult = actor.solve()
        act_result = actor.result

        exp_fresult = data.expect.func_result

        assert act_fresult == exp_fresult
        assert_result_schema(act_result, ComplexResult.Attributes.CAVEATS)
        assert_result_caveats(
            act_result[ComplexResult.Attributes.CAVEATS], data.expect.caveats
        )

        if utils.test_expects_exception(data):
            assert_result_schema(act_result, ComplexResult.Attributes.ERROR)
            assert act_result.has_error
            raise act_result[ComplexResult.Attributes.ERROR]
        else:
            exp_vec = matops.reshape(create_matrix(data.expect.vec_result), (-1, 1))
            exp_iter_count = data.expect.iter_count

            assert_result_schema(
                act_result,
                ComplexResult.Attributes.RESULT_VECTOR,
                ComplexResult.Attributes.ITERATIONS,
            )

            assert matops.almost_equal(
                act_result[ComplexResult.Attributes.RESULT_VECTOR], exp_vec
            )
            assert_iterations_count(
                act_result[ComplexResult.Attributes.ITERATIONS], exp_iter_count
            )


"""
Full examples
https://en.wikipedia.org/wiki/Jacobi_method#Examples



"""
