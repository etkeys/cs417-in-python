from os import path
from tempfile import gettempdir

import pytest

import src.matrix_operations as matops
import tests.utils as utils
from . import assert_call_app_main, setup as pkg_setup, teardown as pkg_teardown

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

    if exp_size is None:
        # size doesn't exist, we weren't supposed to do this assert so
        # just silently continue
        return

    items = matops.load_files(directory, None, False)
    print(items)

    assert matops.is_square(items["matA"])
    assert matops.count_rows(items["matA"]) == exp_size
    assert_vector(items["matb"])
    assert_vector(items["matsoln"])
    if "omega" in items:
        assert isinstance(items["omega"], float)


@pytest.fixture
def base_args():
    return ["src", "make"]


def test_with_directory(name, data, exception, mocker, base_args):
    pkg_setup(data)

    inp = data.input
    base_args.extend(inp)

    exp_code = data.expect.returncode
    assert_call_app_main(mocker, base_args, exp_code)

    exp_dir = data.expect.get("dir_exists")
    assert True if exp_dir is None else utils.check_dir_exists(exp_dir)


def test_with_from_legacy(name, data, exception, mocker, base_args, data_dir):
    pkg_setup(data)

    inp_args = data.input.args
    if "data_dir" in data.input:
        inp_args.append(path.join(data_dir, data.input.data_dir))
    if "args2" in data.input:
        inp_args.extend(data.input.args2)
    base_args.extend(inp_args)

    exp_code = data.expect.returncode
    assert_call_app_main(mocker, base_args, exp_code)

    exp_size = data.expect.get("matrix_size")
    _assert_expected_matrix_sizes(_DEFAULT_OUT_DIR, exp_size)

    pkg_teardown(data)


def test_with_size(name, data, exception, mocker, base_args):
    inp = data.input
    base_args.extend(inp)

    exp_code = data.expect.returncode
    assert_call_app_main(mocker, base_args, exp_code)

    exp_size = data.expect.get("size")
    _assert_expected_matrix_sizes(_DEFAULT_OUT_DIR, exp_size)
