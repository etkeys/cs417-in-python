from contextlib import contextmanager
from os import path

import pytest
import yaml

import tests.utils as utils


@contextmanager
def does_not_raise():
    yield


def _get_test_data_file(module, function):
    def module_name_to_path():
        return path.join(*(module.__name__.split(".")))

    def strip_prefix(string):
        return string[len("test_") :]

    mod_path_head, mod_path_tail = path.split(module_name_to_path())

    parts = mod_path_head.split("/")
    parts.insert(1, "data")
    parts.append(strip_prefix(mod_path_tail))
    parts.append(f"{strip_prefix(function.__name__)}.yml")

    ret_full = path.join(*parts)
    ret_parent = path.dirname(ret_full)

    return (ret_full, ret_parent)


def pytest_addoption(parser):
    parser.addoption(
        "--withslow", action="store_true", default=False, help="include slow tests"
    )


def pytest_generate_tests(metafunc):
    def generate_test(item):
        def item_has_mark(mark_name):
            return (
                "pytest" in item
                and "mark" in item.pytest
                and mark_name in item.pytest.mark
            )

        if utils.test_expects_exception(item):
            err = utils.expect_to_error(item.expect)
            mgr = pytest.raises(err)
        else:
            mgr = does_not_raise()

        if item_has_mark("slow") and not metafunc.config.getoption("--withslow"):
            ret = pytest.param(
                item.name,
                item,
                mgr,
                marks=pytest.mark.skip(reason="requested to skip slow test"),
            )
        elif item_has_mark("xfail"):
            ret = pytest.param(
                item.name, item, mgr, marks=pytest.mark.xfail(strict=True)
            )
        else:
            ret = (item.name, item, mgr)

        return ret

    full_path, parent_dir = _get_test_data_file(metafunc.module, metafunc.function)

    with open(full_path, "r") as fp:
        data = yaml.safe_load(fp.read())

    data = [utils.RecursiveNamespace(**item) for item in data]
    tests = [generate_test(test) for test in data]
    metafunc.parametrize("name, data, exception", tests)

    if "data_dir" in metafunc.fixturenames:
        metafunc.parametrize("data_dir", [parent_dir], scope="function")
