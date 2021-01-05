
import subprocess

import tests.utils as utils

class TestMakeEntryPoint(utils.TestCaseBase):

    @property
    def _initial_command(self):
        return ['python', '-m', 'src', 'make']
    
    @property
    def root_test_data_path(self):
       return ['entry_points', 'make']

    def _setup_with_directory(self, data):
        for step in data:
            if 'delete_dir' in step:
                utils.delete_dir(step.delete_dir)

    def test_with_size(self):
        def expect_else(data):
            inp = data.input
            exp = data.expect

            cmd = self._initial_command
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
            file='with_size',
            expect_else=expect_else
        )

    def test_with_directory(self):
        def expect_throws(data):
            if 'setup' in data:
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
                stdout=subprocess.DEVNULL)
        
        def expect_else(data):
            if 'setup' in data:
                self._setup_with_directory(data.setup)

            inp = data.input
            exp_code = data.expect.returncode
            exp_dir = data.expect.dir_exists

            cmd = self._initial_command
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
            file='with_directory',
            expect_throws=expect_throws,
            expect_else=expect_else
        )
