# from copy import deepcopy as dcopy
from os import path
from tempfile import gettempdir

import pytest

import src.matrix_operations as matops
import tests.utils as utils
from . import call_subprocess, setup as pkg_setup, teardown as pkg_teardown

pytestmark = pytest.mark.subprocess

_DEFAULT_OUT_DIR = path.join(gettempdir(), "makemat")


def _assert_expected_matrix_sizes(directory, exp_size):
    def assert_vector(vector):
        assert matops.is_vector(vector)
        if matops.is_vvector(vector):
            assert matops.count_rows(vector) == exp_size
        else:
            assert matops.count_columns(vector) == exp_size

    if not path.isdir(directory):
        # directory doesn't exist, we weren't supposed to do this assert so
        # just silently continue
        return

    matA, matb, matsoln = matops.load_files(directory)

    assert matops.is_matrix(matA)
    assert matops.is_square(matA)
    assert matops.count_rows(matA) == exp_size
    assert_vector(matb)
    assert_vector(matsoln)


@pytest.fixture
def base_command():
    return ["python", "-m", "src", "make"]


def test_with_directory(name, data, exception, base_command):
    pkg_setup(data)

    inp = data.input
    base_command.extend(inp)

    with exception:
        act_code, act_output = call_subprocess(base_command)
        # print(act_output.stdout)
        # print(act_output.stderr)

        exp_code = data.expect.returncode
        assert act_code == exp_code

        if hasattr(data.expect, "dir_exists"):
            exp_dir = data.expect.dir_exists
            assert utils.check_dir_exists(exp_dir)


def test_with_from_legacy(name, data, exception, base_command, data_dir):
    pkg_setup(data)

    inp_args = data.input.args
    if "data_dir" in data.input:
        inp_args.append(path.join(data_dir, data.input.data_dir))
    if "args2" in data.input:
        inp_args.extend(data.input.args2)

    base_command.extend(inp_args)

    with exception:
        act_code, act_output = call_subprocess(base_command)
        # print(act_output.stdout)
        # print(act_output.stderr)

        exp_code = data.expect.returncode
        exp_dir = _DEFAULT_OUT_DIR
        exp_size = data.expect.matrix_size

        assert act_code == exp_code
        _assert_expected_matrix_sizes(exp_dir, exp_size)

    pkg_teardown(data)


def test_with_size(name, data, exception, base_command):
    inp = data.input
    base_command.extend(inp)

    with exception:
        act_code, _ = call_subprocess(base_command)

        exp_code = data.expect.returncode

        assert act_code == exp_code

        if exp_code == 0:
            exp_dir = _DEFAULT_OUT_DIR
            exp_size = data.expect.size
            _assert_expected_matrix_sizes(exp_dir, exp_size)
