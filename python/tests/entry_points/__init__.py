import subprocess

import tests.utils as utils


def _delete_dir(step):
    if "delete_dir" in step:
        utils.delete_dir(step.delete_dir)


def call_subprocess(cmd_parts):
    act = subprocess.run(
        cmd_parts, stderr=subprocess.PIPE, stdout=subprocess.PIPE, encoding="utf-8"
    )

    return (act.returncode, {"stderr": act.stderr, "stdout": act.stdout})


def setup(data):
    if "setup" in data:
        for step in data.setup:
            _delete_dir(step)


def teardown(data):
    if "teardown" in data:
        for step in data.teardown:
            _delete_dir(step)
