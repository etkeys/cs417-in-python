import tests.utils as utils


def _delete_dir(step):
    if "delete_dir" in step:
        utils.delete_dir(step.delete_dir)


def setup(data):
    if "setup" in data:
        for step in data.setup:
            _delete_dir(step)


def teardown(data):
    if "teardown" in data:
        for step in data.teardown:
            _delete_dir(step)
