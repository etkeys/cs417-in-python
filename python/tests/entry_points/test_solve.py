from os import path

import pytest

from . import call_subprocess


@pytest.fixture
def base_command():
    return ["python", "-m", "src", "solve"]


def test_no_options(name, data, exception, base_command, data_dir):
    inp_args = getattr(data.input, "args", None)
    inp_args2 = getattr(data.input, "args2", None)
    inp_dir = getattr(data.input, "dir", None)

    if inp_args:
        base_command.extend(inp_args)
    if inp_dir:
        base_command.append(path.join(data_dir, inp_dir))
    if inp_args2:
        base_command.extend(inp_args2)
    # print(base_command)

    with exception:
        act_code, _ = call_subprocess(base_command)

        exp = data.expect

        assert act_code == exp


def test_with_check(name, data, exception, base_command, data_dir):
    inp_args2 = data.input.args2
    inp_dir = getattr(data.input, "dir", None)

    if inp_dir:
        base_command.append(path.join(data_dir, inp_dir))
    if inp_args2:
        base_command.extend(inp_args2)
    # print(base_command)

    with exception:
        act_code, _ = call_subprocess(base_command)

        exp = data.expect

        assert act_code == exp
