from pydoc import locate
import os
import shutil

from .recursive_namespace import RecursiveNamespace


def check_dir_exists(path):
    return os.path.isdir(path)


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
    if expect == "throws":
        return ValueError
    ret = string_to_type(getattr(expect, "throws", ""), ValueError)
    return ret


def string_to_type(type_string, default=None):
    _default = default if default is not None else None
    ret = None
    if type_string is not None and len(type_string) > 0:
        ret = locate(type_string)
    if ret is None:
        ret = _default
    return ret


def test_expects_exception(test_data):
    return hasattr(test_data.expect, "throws") or test_data.expect == "throws"

