
from .recursive_namespace import RecursiveNamespace

def load_test_data(name):
    from os import path
    import yaml

    file = path.abspath(path.join('tests', 'test_data', '%s.yml' % name))
    with open(file, 'r') as fp:
        data = yaml.safe_load(fp.read())
    
    ret = [RecursiveNamespace(**item) for item in data]
    return ret