import sys

import pytest

from src.main import main as app_main
import tests.utils as utils


def _delete_dir(step):
    if "delete_dir" in step:
        utils.delete_dir(step.delete_dir)


def assert_call_app_main(mocker, base_args, exp_ret_code):
    mocker.patch.object(sys, "argv", base_args)
    with pytest.raises(SystemExit) as sysexit:
        app_main()

    assert sysexit.type == SystemExit
    assert sysexit.value.code == exp_ret_code


def setup(data):
    if "setup" in data:
        for step in data.setup:
            _delete_dir(step)


def teardown(data):
    if "teardown" in data:
        for step in data.teardown:
            _delete_dir(step)
