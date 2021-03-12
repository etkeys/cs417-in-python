import pytest

import src.matrix_operations as matops
from src.solvers import Result, ResultAttributes
from tests.utils import create_matrix


def assert_iterations_count(act_count, exp_count):
    if isinstance(exp_count, int):
        assert act_count == exp_count
    elif isinstance(exp_count, list):
        assert exp_count[0] <= act_count <= exp_count[1]
    else:
        pytest.fail("exp_count should be int or list of ints.")


def common_test_solve(data, exception, solver):
    """
    Paramerters
    -----
    data : the namespace representation of the yaml data for the test
    exception : the context manager to trap expected exceptions
    solver : static refernce (not instance) of what solver type to test
    """
    inputs = vars(data.input)
    for k, v in inputs.items():
        if k.startswith("mat"):
            inputs[k] = create_matrix(v)

    with exception:
        actor = solver(**inputs)
        act_fresult = actor.solve()
        act_result = actor.result

        exp_fresult = data.expect.func_result

        assert act_fresult == exp_fresult
        assert isinstance(act_result, Result)

        if act_result.has_error:
            raise act_result[ResultAttributes.ERROR]

        exp_vec = matops.reshape(create_matrix(data.expect.vec_result), (-1, 1))
        assert ResultAttributes.RESULT_VECTOR in act_result
        assert matops.almost_equal(act_result[ResultAttributes.RESULT_VECTOR], exp_vec)

        if not "iter_count" in data.expect:
            return

        exp_iter_count = data.expect.iter_count
        assert ResultAttributes.ITERATIONS in act_result
        assert_iterations_count(act_result[ResultAttributes.ITERATIONS], exp_iter_count)
