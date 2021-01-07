import numpy as np

import src.matrix_operations as matops
import tests.utils as utils


class TestMatrixOperations(utils.TestCaseBase):
    @property
    def root_test_data_path(self):
        return ["matrix"]

    def test_create_augmented(self):
        def expect_throws(test):
            A = np.array(test.input.A)
            b = np.array(test.input.b)
            self.assertRaises(ValueError, matops.create_augmented, A, b)

        def expect_else(test):
            A = np.array(test.input.A)
            b = np.array(test.input.b)
            A_orig = matops.deepcopy(A)
            b_orig = matops.deepcopy(b)
            act = matops.create_augmented(A, b)
            exp = np.array(test.expect)

            self.assertTrue(np.equal(act, exp).all())
            self.assertEqual(A.shape, A_orig.shape)
            self.assertEqual(b.shape, b_orig.shape)

        utils.run_test(
            self,
            file="create_augmented",
            expect_throws=expect_throws,
            expect_else=expect_else,
        )

    def test_create_random(self):
        def expect_throws(test):
            self.assertRaises(
                ValueError,
                matops.create_random,
                test.input.size,
                test.input.single_column,
            )

        def expect_else(test):
            act = matops.create_random(test.input.size, test.input.single_column)
            self.assertEqual(act.shape, (test.expect.rows, test.expect.columns))

        utils.run_test(
            self,
            file="create_random",
            expect_throws=expect_throws,
            expect_else=expect_else,
        )

    def test_create_random_validate_values(self):
        def expect_else(test):
            act = matops.create_random(test.input.size, test.input.single_column)
            self.assertEqual(act.dtype, np.dtype(np.float64))

            mask = np.logical_and(act <= test.expect.upper, act >= test.expect.lower)
            if test.expect.any:
                self.assertTrue(not np.all(mask), "All values are in range.")
            else:
                self.assertTrue(np.all(mask), "One value is out of range.")

        utils.run_test(
            self, file="create_random_validate_values", expect_else=expect_else
        )

    def test_create_random_diag_dominate(self):
        def expect_else(test):
            act = matops.create_random_diagonal_dominate(test.input.size)
            self.assertEqual(act.shape, (test.expect.size, test.expect.size))

            diag = act.diagonal()
            bigs = act.max(0)
            self.assertTrue(np.equal(diag, bigs).all())

        utils.run_test(
            self, file="create_random_diag_dominate", expect_else=expect_else
        )

    def test_create_zeros(self):
        def expect_throws(test):
            self.assertRaises(
                ValueError, matops.create_zeros, test.input.rows, test.input.columns
            )

        def expect_else(test):
            exp = np.array(test.expect.mat)
            act = matops.create_zeros(test.input.rows, test.input.columns)

            if not test.expect.columns:
                self.assertEqual(act.shape, (test.expect.rows,))
            else:
                self.assertEqual(act.shape, (test.expect.rows, test.expect.columns))
            self.assertTrue(np.allclose(act, exp))

        utils.run_test(
            self,
            file="create_zeros",
            expect_throws=expect_throws,
            expect_else=expect_else,
        )

    def test_is_augmented(self):
        def expect_else(test):
            inp = np.array(test.input)
            act = matops.is_augmented(inp)
            self.assertEqual(act, test.expect)

        utils.run_test(self, file="is_augmented", expect_else=expect_else)

    def test_is_in_reduced_row_echelon(self):
        def expect_else(test):
            inp = np.array(test.input)
            act = matops.is_in_reduced_row_echelon(inp)
            self.assertEqual(act, test.expect)

        utils.run_test(self, file="is_in_reduced_row_echelon", expect_else=expect_else)

    def test_is_singular(self):
        def expect_else(test):
            # print('')
            # print(test.name)
            inp = np.array(test.input)
            act = matops.is_singular(inp)
            self.assertEqual(act, test.expect)

        utils.run_test(self, file="is_singular", expect_else=expect_else)

    def test_is_square(self):
        def expect_else(test):
            inp = np.array(test.input)
            act = matops.is_square(inp)
            self.assertEqual(act, test.expect)

        utils.run_test(self, file="is_square", expect_else=expect_else)

    def test_multiply(self):
        def expect_else(test):
            a = np.array(test.input.a)
            b = np.array(test.input.b)
            act = matops.multiply(a, b)
            exp = np.array(test.expect)
            self.assertTrue(np.equal(act, exp).all())

        utils.run_test(self, file="multiply", expect_else=expect_else)

    def test_multiply_row_by_scalar(self):
        def expect_else(test):
            inp = np.array(test.input.mat)
            exp = np.array(test.expect.mat)
            act = matops.multiply_row_by_scalar(
                inp, test.input.row, test.input.scalar, test.input.inplace
            )

            # print('')
            # print(test.name)
            # print(inp)
            # print(exp)
            # print(act)

            self.assertTrue(np.allclose(act, exp))
            self.assertEqual(np.equal(act, inp).all(), test.expect.inp_match_act)

        utils.run_test(self, file="multiply_row_by_scalar", expect_else=expect_else)

    def test_percent_error(self):
        def expect_else(test):
            # print('')
            # print(test.name)
            inp_act = np.array(test.input.act).reshape(-1, 1)
            inp_exp = np.array(test.input.exp).reshape(-1, 1)
            exp = test.expect if test.expect != "np.inf" else np.inf
            act = matops.percent_error(inp_act, inp_exp)

            # print(exp)
            # print(act)

            self.assertTrue(act >= 0.0)
            self.assertAlmostEqual(act, exp)

        utils.run_test(self, file="percent_error", expect_else=expect_else)

    def test_set_row_diagonal_to_one(self):
        def expect_else(test):
            inp = np.array(test.input.mat)
            exp = np.array(test.expect.mat)
            act = matops.set_row_diagonal_to_one(
                inp, test.input.row, test.input.inplace
            )

            # print('')
            # print(test.name)
            # print(inp)
            # print(exp)
            # print(act)

            self.assertTrue(np.allclose(act, exp))
            self.assertEqual(np.equal(act, inp).all(), test.expect.inp_match_act)

        utils.run_test(self, file="set_row_diag_to_one", expect_else=expect_else)

    def test_set_rows_below_to_zero(self):
        def expect_else(test):
            inp = np.array(test.input.mat)
            exp = np.array(test.expect.mat)
            act = matops.set_rows_below_to_zero(
                inp, test.input.source_row, test.input.inplace
            )

            self.assertTrue(np.allclose(act, exp))
            self.assertEqual(np.equal(act, inp).all(), test.expect.inp_match_act)

        utils.run_test(self, file="set_rows_below_to_zero", expect_else=expect_else)

    def test_subtract_scalar_row_from_row(self):
        def expect_else(test):
            inp = np.array(test.input.mat)
            exp = np.array(test.expect.mat)
            act = matops.subtract_scalar_row_from_row(
                inp, test.input.source_row, test.input.affect_row, test.input.inplace
            )

            # print('')
            # print(inp)
            # print(exp)
            # print(act)

            self.assertTrue(np.allclose(act, exp))
            self.assertEqual(np.equal(act, inp).all(), test.expect.inp_match_act)

        utils.run_test(
            self, file="subtract_scalar_row_from_row", expect_else=expect_else
        )

    def test_swap_largest_pivot_to_top(self):
        def expect_else(test):
            inp = np.array(test.input.mat)
            exp = np.array(test.expect.mat)
            act = matops.swap_largest_pivot_to_top(
                inp, test.input.pivot, test.input.inplace
            )

            # print('')
            # print(inp)
            # print(exp)
            # print(act)

            self.assertTrue(np.equal(act, exp).all())
            self.assertEqual(np.equal(act, inp).all(), test.expect.inp_match_act)

        utils.run_test(self, file="swap_largest_pivot_to_top", expect_else=expect_else)

    def test_to_reduced_row_echelon(self):
        def expect_else(test):
            inp = np.array(test.input)
            exp = np.array(test.expect)
            act = matops.to_reduced_row_echelon(inp)

            # print('')
            # print(inp)
            # print(exp)
            # print(act)

            self.assertTrue(np.allclose(act, exp))

        utils.run_test(self, file="to_reduced_row_echelon", expect_else=expect_else)

    def test_two_norm_of_error(self):
        def expect_else(test):
            inpA = np.array(test.input.matA)
            inpb = np.array(test.input.matb)
            inpx = np.array(test.input.matx)
            exp = test.expect
            act = matops.two_norm_of_error(inpA, inpb, inpx)

            self.assertAlmostEqual(act, exp)

        utils.run_test(self, file="two_norm_of_error", expect_else=expect_else)
