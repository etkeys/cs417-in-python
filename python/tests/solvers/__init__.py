
import pytest

from src.solvers.solver import ComplexResult
from tests.utils import string_to_type

def assert_result_caveats(act_cavs, raw_exp_cavs):
    def caveats_path(member):
        return f"src.solvers.solver.ComplexResult.Caveats.{member}"

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