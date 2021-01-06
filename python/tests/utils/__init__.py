
import os
import shutil
import yaml

from .recursive_namespace import RecursiveNamespace
from .test_case_base import TestCaseBase

def check_dir_exists(path):
    return os.path.isdir(path)

def build_path_to_test_data(fname=None, *args):
    root_parts = ['tests', 'test_data']
    root_parts.extend(args)
    if fname:
        root_parts.append(f'{fname}.yml')
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


def load_test_data(name, *args):
    file = build_path_to_test_data(name, *args)
    with open(file, 'r') as fp:
        data = yaml.safe_load(fp.read())

    ret = [RecursiveNamespace(**item) for item in data]
    return ret

def run_test(testcase, **kwargs):
    data_file = kwargs['file']
    path_parts_to_data_file = getattr(testcase, 'root_test_data_path', [])
    data = load_test_data(data_file, *path_parts_to_data_file)
    for test in data:
        with testcase.subTest('Test: %s' % test.name):
            if isinstance(test.expect, str) and test.expect == 'throws':
                if 'expect_throws' in kwargs:
                    kwargs['expect_throws'](test)
                else:
                    testcase.fail("Condition not implemented!")
            else:
                kwargs['expect_else'](test)
