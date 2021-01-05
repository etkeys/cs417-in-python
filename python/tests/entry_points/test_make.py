
from copy import deepcopy
from os import path
import subprocess
import unittest

import tests.utils as utils

class TestMakeEntryPoint(unittest.TestCase):

    _init_cmd_parts = ['python', '-m', 'src', 'make']

    @property
    def initial_command(self):
        return deepcopy(self._init_cmd_parts)

    def get_data_for_test_path(self, test_name):
        return path.join('entry_points', 'make', test_name)

    def _setup_with_directory(self, data):
        for step in data:
            if 'delete_dir' in step:
                utils.delete_dir(step.delete_dir)

    def test_with_size(self):
        def expect_else(data):
            inp = data.input
            exp = data.expect

            cmd = self.initial_command
            cmd.extend(inp)
            act = subprocess.run(
                cmd,
                stderr=subprocess.DEVNULL,
                stdout=subprocess.DEVNULL)

            if exp == 0:
                self.assertTrue(
                    act.returncode == 0,
                    'Call should produce success exit code')
            else:
                self.assertFalse(
                    act.returncode == 0,
                    'Call should produce non-success exit code')

        utils.run_test(
            self,
            file=self.get_data_for_test_path('with_size'),
            expect_else=expect_else
        )

    def test_with_directory(self):
        def expect_throws(data):
            if 'setup' in data:
                self._setup_with_directory(data.setup)

            inp = data.input

            cmd = self.initial_command
            cmd.extend(inp)

            self.assertRaises(
                subprocess.CalledProcessError,
                subprocess.run,
                cmd,
                check=True,
                stderr=subprocess.DEVNULL,
                stdout=subprocess.DEVNULL)
        
        def expect_else(data):
            if 'setup' in data:
                self._setup_with_directory(data.setup)

            inp = data.input
            exp_code = data.expect.returncode
            exp_dir = data.expect.dir_exists

            cmd = self.initial_command
            cmd.extend(inp)

            act = subprocess.run(
                cmd,
                stderr=subprocess.PIPE,
                stdout=subprocess.PIPE)
            
            act_dir_exists = utils.check_dir_exists(exp_dir)

            self.assertTrue(
                act.returncode == exp_code,
                'Expected return code %i, got %i' % (exp_code, act.returncode,))
            self.assertTrue(
                act_dir_exists,
                'Expect dir "%s" exists, got %r' % (exp_dir, act_dir_exists))

        utils.run_test(
            self,
            file=self.get_data_for_test_path('with_directory'),
            expect_throws=expect_throws,
            expect_else=expect_else
        )
