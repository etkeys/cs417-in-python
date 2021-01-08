from os import path
import subprocess

import tests.utils as utils


class TestSolveEntryPoint(utils.TestCaseBase):
    @property
    def _initial_command(self):
        return ["python", "-m", "src", "solve"]

    @property
    def root_test_data_path(self):
        return ["entry_points", "solve"]

    def _setup_with_directory(self, data):
        for step in data:
            if "delete_dir" in step:
                utils.delete_dir(step.delete_dir)

    def test_no_options(self):
        def expect_else(test):
            inp = test.input
            exp = test.expect

            cmd = self._initial_command
            if inp:
                subtest_dir = self.root_test_data_path
                subtest_dir.append(inp)
                subtest_dir = utils.build_path_to_test_data(None, *subtest_dir)
                cmd.append(subtest_dir)

            act = subprocess.run(
                cmd, stderr=subprocess.PIPE, stdout=subprocess.PIPE, encoding="utf-8"
            )

            self.assertTrue(
                act.returncode == exp,
                "Expected return code %i, got %i\n%s\n%s"
                % (exp, act.returncode, act.stdout, act.stderr),
            )

        utils.run_test(self, file="no_options", expect_else=expect_else)

    def test_with_check(self):
        def expect_else(test):
            inp_args = test.input.args
            inp_path = test.input.path
            exp = test.expect

            cmd = self._initial_command
            if inp_path:
                subtest_dir = self.root_test_data_path
                subtest_dir.append(inp_path)
                subtest_dir = utils.build_path_to_test_data(None, *subtest_dir)
                cmd.append(subtest_dir)

            cmd.extend(inp_args)
            act = subprocess.run(
                cmd, stderr=subprocess.PIPE, stdout=subprocess.PIPE, encoding="utf-8"
            )

            self.assertTrue(
                act.returncode == exp,
                "Expected return code %i, got %i\n%s\n%s"
                % (exp, act.returncode, act.stdout, act.stderr),
            )

        utils.run_test(self, file="with_check", expect_else=expect_else)
