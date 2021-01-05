
import os
import shutil

from .recursive_namespace import RecursiveNamespace
from .test_case_base import TestCaseBase

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


def load_test_data(name):
    from os import path
    import yaml

    file = path.abspath(path.join('tests', 'test_data', '%s.yml' % name))
    with open(file, 'r') as fp:
        data = yaml.safe_load(fp.read())

    ret = [RecursiveNamespace(**item) for item in data]
    return ret

def run_test(testcase, **kwargs):
    data_file = kwargs['file']
    if hasattr(testcase, 'root_test_data_path'):
        data_file = os.path.join(*(testcase.root_test_data_path + [data_file]))
    data = load_test_data(data_file)
    for test in data:
        with testcase.subTest('Test: %s' % test.name):
            if isinstance(test.expect, str) and test.expect == 'throws':
                if 'expect_throws' in kwargs:
                    kwargs['expect_throws'](test)
                else:
                    testcase.fail("Condition not implemented!")
            else:
                kwargs['expect_else'](test)