from pydoc import locate
import os
import shutil
import yaml

from .recursive_namespace import RecursiveNamespace
from .test_case_base import TestCaseBase


def check_dir_exists(path):
    return os.path.isdir(path)


def build_path_to_test_data(fname=None, *args):
    root_parts = ["tests", "test_data"]
    root_parts.extend(args)
    if fname:
        root_parts.append(f"{fname}.yml")
    return os.path.join(*root_parts)


def delete_dir(path):
    def delete_dir_single(single_path):
        if check_dir_exists(single_path):
            shutil.rmtree(single_path)

    if isinstance(path, list):
        for p in path:
            delete_dir_single(p)
    else:
        delete_dir_single(path)


def expect_to_error(expect):
    ret = string_to_type(getattr(expect, "throws", ""), ValueError)
    return ret


def load_test_data(name, *args):
    file = build_path_to_test_data(name, *args)
    with open(file, "r") as fp:
        data = yaml.safe_load(fp.read())

    ret = [RecursiveNamespace(**item) for item in data]
    return ret


def run_test(testcase, **kwargs):
    def do_throws(data):
        if "expect_throws" in kwargs:
            kwargs["expect_throws"](data)
        else:
            testcase.fail("Condition not implemented!")

    data_file = kwargs["file"]
    path_parts_to_data_file = getattr(testcase, "root_test_data_path", [])
    data = load_test_data(data_file, *path_parts_to_data_file)
    for test in data:
        with testcase.subTest("Test: %s" % test.name):
            if hasattr(test.expect, "throws"):
                do_throws(test)

            elif isinstance(test.expect, str) and test.expect == "throws":
                do_throws(test)

            else:
                kwargs["expect_else"](test)


def string_to_type(type_string, default=None):
    _default = default if default is not None else None
    ret = None
    if type_string is not None and len(type_string) > 0:
        ret = locate(type_string)
    if ret is None:
        ret = _default
    return ret
