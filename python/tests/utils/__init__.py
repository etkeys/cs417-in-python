from pydoc import locate
import os
import re
import shutil

import numpy as np

from src.exceptions import DivergentSolution
from tests import _SUBSITUTION_MAPPING
from .recursive_namespace import RecursiveNamespace


def create_matrix(mat_def):
    ret = np.array(mat_def, dtype=float) if mat_def is not None else None
    return ret


def check_dir_exists(path):
    return os.path.isdir(path)


def expand_data_variables(raw_val):
    if raw_val is None:
        return raw_val
    raw_matches = re.findall(r"\{\{ \w+ \}\}", raw_val)
    tokens = [re.findall(r"\w+", m)[0] for m in raw_matches]

    no_mapping = set(tokens) - _SUBSITUTION_MAPPING.keys()
    if len(no_mapping) > 0:
        raise KeyError(
            f"The following tokens have no substitution mapping: {list(no_mapping)}."
        )

    ret = raw_val
    for m, t in zip(raw_matches, tokens):
        ret = ret.replace(m, _SUBSITUTION_MAPPING[t])
    return ret


def delete_dir(path):
    def delete_dir_single(single_path):
        target = expand_data_variables(single_path)
        if check_dir_exists(target):
            shutil.rmtree(target)

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
