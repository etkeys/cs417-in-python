from os import path

import pytest

from . import assert_call_app_main


@pytest.fixture
def base_args():
    return ["src", "solve"]


def _do_common_test(data, mocker, base_args, data_dir):
    inp_args = data.input.get("args", [])
    inp_args2 = data.input.get("args2", [])
    inp_dir = data.input.get("dir")

    base_args.extend(inp_args)
    if inp_dir:
        base_args.append(path.join(data_dir, inp_dir))
    base_args.extend(inp_args2)

    exp_code = data.expect
    assert_call_app_main(mocker, base_args, exp_code)


def test_no_options(name, data, exception, mocker, base_args, data_dir):
    _do_common_test(data, mocker, base_args, data_dir)


def test_with_check(name, data, exception, mocker, base_args, data_dir):
    _do_common_test(data, mocker, base_args, data_dir)


def test_with_guess(name, data, exception, mocker, base_args, data_dir):
    _do_common_test(data, mocker, base_args, data_dir)


def test_with_omega(name, data, exception, mocker, base_args, data_dir):
    _do_common_test(data, mocker, base_args, data_dir)
