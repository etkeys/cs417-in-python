import pytest


def assert_iterations_count(act_count, exp_count):
    if isinstance(exp_count, int):
        assert act_count == exp_count
    elif isinstance(exp_count, list):
        assert exp_count[0] <= act_count <= exp_count[1]
    else:
        pytest.fail("exp_count should be int or list of ints.")
