from os import path
import subprocess

import src.matrix_operations as matops
import tests.utils as utils


class TestMakeEntryPoint(utils.TestCaseBase):
    @property
    def _initial_command(self):
        return ["python", "-m", "src", "make"]

    @property
    def root_test_data_path(self):
        return ["entry_points", "make"]

    # TODO this is used by other methods than "with_directory" make more generic
    def _setup_with_directory(self, data):
        for step in data:
            if "delete_dir" in step:
                utils.delete_dir(step.delete_dir)

    def test_with_directory(self):
        def expect_throws(data):
            if "setup" in data:
                self._setup_with_directory(data.setup)

            inp = data.input

            cmd = self._initial_command
            cmd.extend(inp)

            self.assertRaises(
                subprocess.CalledProcessError,
                subprocess.run,
                cmd,
                check=True,
                stderr=subprocess.DEVNULL,
                stdout=subprocess.DEVNULL,
            )

        def expect_else(data):
            if "setup" in data:
                self._setup_with_directory(data.setup)

            inp = data.input
            exp_code = data.expect.returncode
            exp_dir = data.expect.dir_exists

            cmd = self._initial_command
            cmd.extend(inp)

            act = subprocess.run(
                cmd, stderr=subprocess.PIPE, stdout=subprocess.PIPE, encoding="utf-8"
            )
            # print(act.stdout)
            # print(act.stderr)

            act_dir_exists = utils.check_dir_exists(exp_dir)

            self.assertTrue(
                act.returncode == exp_code,
                "Expected return code %i, got %i"
                % (
                    exp_code,
                    act.returncode,
                ),
            )
            self.assertTrue(
                act_dir_exists,
                'Expect dir "%s" exists, got %r' % (exp_dir, act_dir_exists),
            )

        utils.run_test(
            self,
            file="with_directory",
            expect_throws=expect_throws,
            expect_else=expect_else,
        )

    def test_with_from_legacy(self):
        def expect_else(data):
            if "setup" in data:
                self._setup_with_directory(data.setup)

            inp = data.input
            exp_code = data.expect.returncode
            # HACK this isn't portable
            exp_dir = "/tmp/makemat"
            exp_size = data.expect.matrix_size

            cmd = self._initial_command
            cmd.extend(inp)

            # TODO can this subprocess call be refactor to be reusable?
            act = subprocess.run(
                cmd, stderr=subprocess.PIPE, stdout=subprocess.PIPE, encoding="utf-8"
            )
            # print(act.stdout)
            # print(act.stderr)

            self._assert_expected_returncode(act.returncode, exp_code)
            self._assert_expected_matrix_sizes(exp_dir, exp_size)

            if "teardown" in data:
                self._custom_teardown(data.teardown)

        utils.run_test(
            self,
            file="with_from_legacy",
            expect_else=expect_else,
        )

    def test_with_size(self):
        def expect_else(data):
            inp = data.input
            exp = data.expect

            cmd = self._initial_command
            cmd.extend(inp)
            act = subprocess.run(
                cmd, stderr=subprocess.DEVNULL, stdout=subprocess.DEVNULL
            )

            if exp == 0:
                self.assertTrue(
                    act.returncode == 0, "Call should produce success exit code"
                )
            else:
                self.assertFalse(
                    act.returncode == 0, "Call should produce non-success exit code"
                )

        utils.run_test(self, file="with_size", expect_else=expect_else)

    def _assert_expected_matrix_sizes(self, directory, exp_size):
        def assert_vector(vector):
            self.assertTrue(matops.is_vector(vector))
            if matops.is_vvector(vector):
                self.assertEqual(matops.count_rows(vector), exp_size)
            else:
                self.assertEqual(matops.count_columns(vector), exp_size)

        if not path.isdir(directory):
            # directory doesn't exist, we weren't supposed to do this assert so
            # just silently continue
            return

        matA, matb, matsoln = matops.load_files(directory)

        self.assertTrue(matops.is_matrix(matA))
        self.assertEqual(matops.count_rows(matA), exp_size)
        self.assertEqual(matops.count_columns(matA), exp_size)
        assert_vector(matb)
        assert_vector(matsoln)

    def _assert_expected_returncode(self, act, exp):
        self.assertTrue(
            act == exp,
            "Expected return code %i, got %i"
            % (
                exp,
                act,
            ),
        )

    def _custom_teardown(self, data):
        for step in data:
            if "delete_dir" in step:
                utils.delete_dir(step.delete_dir)
