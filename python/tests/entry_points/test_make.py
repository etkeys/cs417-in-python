# from copy import deepcopy as dcopy
from os import path
import subprocess
from tempfile import gettempdir

import pytest

import src.matrix_operations as matops
import tests.utils as utils

_DEFAULT_OUT_DIR = path.join(gettempdir(), "makemat")
_NULL = subprocess.DEVNULL
_PIPE = subprocess.PIPE


def _assert_expected_matrix_sizes(directory, exp_size):
    def assert_vector(vector):
        # self.assertTrue(matops.is_vector(vector))
        assert matops.is_vector(vector)
        if matops.is_vvector(vector):
            # self.assertEqual(matops.count_rows(vector), exp_size)
            assert matops.count_rows(vector) == exp_size
        else:
            # self.assertEqual(matops.count_columns(vector), exp_size)
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


def _custom_teardown(data):
    for step in data:
        if "delete_dir" in step:
            utils.delete_dir(step.delete_dir)


# TODO this is used by other methods than "with_directory" make more generic
def _setup_with_directory(data):
    for step in data:
        if "delete_dir" in step:
            utils.delete_dir(step.delete_dir)


@pytest.fixture
def base_command():
    return ["python", "-m", "src", "make"]


def test_with_directory(name, data, exception, base_command):
    if "setup" in data:
        _setup_with_directory(data.setup)

    inp = data.input
    base_command.extend(inp)

    with exception:
        act = subprocess.run(base_command, stderr=_PIPE, stdout=_PIPE, encoding="utf-8")
        # print(act.stdout)
        # print(act.stderr)

        exp_code = data.expect.returncode
        assert act.returncode == exp_code

        if hasattr(data.expect, "dir_exists"):
            exp_dir = data.expect.dir_exists
            assert utils.check_dir_exists(exp_dir)


def test_with_from_legacy(name, data, exception, base_command, data_dir):
    if "setup" in data:
        _setup_with_directory(data.setup)

    inp_args = data.input.args
    if "data_dir" in data.input:
        inp_args.append(path.join(data_dir, data.input.data_dir))
    if "args2" in data.input:
        inp_args.extend(data.input.args2)

    base_command.extend(inp_args)

    with exception:
        # TODO can this subprocess call be refactor to be reusable?
        act = subprocess.run(base_command, stderr=_PIPE, stdout=_PIPE, encoding="utf-8")
        # print(act.stdout)
        # print(act.stderr)

        exp_code = data.expect.returncode
        exp_dir = _DEFAULT_OUT_DIR
        exp_size = data.expect.matrix_size

        assert act.returncode == exp_code
        _assert_expected_matrix_sizes(exp_dir, exp_size)

    if "teardown" in data:
        _custom_teardown(data.teardown)


def test_with_size(name, data, exception, base_command):
    inp = data.input
    base_command.extend(inp)

    with exception:
        act = subprocess.run(base_command, stderr=_NULL, stdout=_NULL)

        exp_code = data.expect.returncode

        assert act.returncode == exp_code

        if exp_code == 0:
            exp_dir = _DEFAULT_OUT_DIR
            exp_size = data.expect.size
            _assert_expected_matrix_sizes(exp_dir, exp_size)


