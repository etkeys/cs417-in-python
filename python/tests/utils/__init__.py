
from .recursive_namespace import RecursiveNamespace

def load_test_data(name):
    from os import path
    import yaml

    file = path.abspath(path.join('tests', 'test_data', '%s.yml' % name))
    with open(file, 'r') as fp:
        data = yaml.safe_load(fp.read())
    
    ret = [RecursiveNamespace(**item) for item in data]
    return ret

def run_test(testcase, **kwargs):
    data = load_test_data(kwargs['file'])
    for test in data:
        with testcase.subTest('Test: %s' % test.name):
            if isinstance(test.expect, str) and test.expect == 'throws':
                if 'expect_throws' in kwargs:
                    kwargs['expect_throws'](test)
                else:
                    testcase.fail("Condition not implemented!")
            else:
                kwargs['expect_else'](test)