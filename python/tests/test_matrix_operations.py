from math import isclose

import numpy as np

import src.matrix_operations as matops


def test_count_columns(name, data, exception):
    inp = np.array(data.input.mat) if hasattr(data.input, "mat") else None

    with exception:
        act = matops.count_columns(inp)

        exp = data.expect

        assert act == exp


def test_count_rows(name, data, exception):
    inp = np.array(data.input.mat) if hasattr(data.input, "mat") else None

    with exception:
        act = matops.count_rows(inp)

        exp = data.expect

        assert act == exp


def test_create_augmented(name, data, exception):
    A = np.array(data.input.A)
    b = np.array(data.input.b)
    A_orig = matops.deepcopy(A)
    b_orig = matops.deepcopy(b)

    with exception:
        act = matops.create_augmented(A, b)

        exp = np.array(data.expect)

        assert np.equal(act, exp).all()
        assert A.shape == A_orig.shape
        assert b.shape == b_orig.shape


def test_create_identity(name, data, exception):
    inp = data.input

    with exception:
        act = matops.create_identity(inp)

        exp = np.array(data.expect)

        assert np.equal(act, exp).all()


def test_create_random(name, data, exception):
    inp_single_column = data.input.single_column
    inp_size = data.input.size

    with exception:
        act = matops.create_random(inp_size, inp_single_column)

        exp_columns = data.expect.columns
        exp_rows = data.expect.rows

        assert act.shape == (exp_rows, exp_columns)


def test_create_random_diag_dominate(name, data, exception):
    inp_size = data.input.size

    with exception:
        act = matops.create_random_diagonal_dominate(inp_size)

        exp_size = data.expect.size

        assert act.shape == (exp_size, exp_size)

        diag = act.diagonal()
        bigs = act.max(0)

        assert np.equal(diag, bigs).all()


def test_create_random_validate_values(name, data, exception):
    inp_single_column = data.input.single_column
    inp_size = data.input.size

    with exception:
        act = matops.create_random(inp_size, inp_single_column)

        assert act.dtype == np.float64

        mask = np.logical_and(act <= data.expect.upper, act >= data.expect.lower)
        act_all = np.all(mask)

        # All values should be within the expected range
        assert act_all != data.expect.any


def test_create_zeros(name, data, exception):
    inp_cols = data.input.columns
    inp_rows = data.input.rows

    with exception:
        act = matops.create_zeros(inp_rows, inp_cols)

        exp = np.array(data.expect.mat)

        assert act.shape == exp.shape
        assert np.allclose(act, exp)


def test_is_augmented(name, data, exception):
    inp = np.array(data.input)

    with exception:
        act = matops.is_augmented(inp)

        assert act == data.expect


def test_is_hvector(name, data, exception):
    inp_as_raw = data.input.as_raw
    inp_value = data.input.value if inp_as_raw else np.array(data.input.value)

    with exception:
        act = matops.is_hvector(inp_value)

        exp = data.expect

        assert act == exp


def test_is_in_crout_l_form(name, data, exception):
    inp = np.array(data.input, dtype=float)

    with exception:
        act = matops.is_in_crout_l_form(inp)

        exp = data.expect

        assert act == exp


def test_is_in_reduced_row_echelon(name, data, exception):
    inp = np.array(data.input)

    with exception:
        act = matops.is_in_reduced_row_echelon(inp)

        assert act == data.expect


def test_is_matrix(name, data, exception):
    inp_as_raw = data.input.as_raw
    inp_value = data.input.value if inp_as_raw else np.array(data.input.value)

    with exception:
        act = matops.is_matrix(inp_value)

        exp = data.expect

        assert act == exp


def test_is_singular(name, data, exception):
    # print('')
    # print(data.name)
    inp = np.array(data.input)

    with exception:
        act = matops.is_singular(inp)

        assert act == data.expect


def test_is_square(name, data, exception):
    inp = np.array(data.input)

    with exception:
        act = matops.is_square(inp)

        assert act == data.expect


def test_is_vector(name, data, exception):
    inp_as_raw = data.input.as_raw
    inp_value = data.input.value if inp_as_raw else np.array(data.input.value)

    with exception:
        act = matops.is_vector(inp_value)

        exp = data.expect

        assert act == exp


def test_is_vvector(name, data, exception):
    inp_as_raw = data.input.as_raw
    inp_value = data.input.value if inp_as_raw else np.array(data.input.value)

    with exception:
        act = matops.is_vvector(inp_value)

        exp = data.expect

        assert act == exp


def test_multiply(name, data, exception):
    inp_matA = np.array(data.input.a)
    inp_matb = np.array(data.input.b)

    with exception:
        act = matops.multiply(inp_matA, inp_matb)

        exp = np.array(data.expect)

        assert np.allclose(act, exp)


def test_multiply_row_by_scalar(name, data, exception):
    # print('')
    # print(data.name)
    inp_inplace = data.input.inplace
    inp_mat = np.array(data.input.mat)
    inp_row = data.input.row
    inp_scalar = data.input.scalar

    with exception:
        act = matops.multiply_row_by_scalar(inp_mat, inp_row, inp_scalar, inp_inplace)

        exp = np.array(data.expect.mat)

        # print(inp)
        # print(exp)
        # print(act)

        assert np.allclose(act, exp)
        # make sure inp matrix was not mutated
        assert np.allclose(act, inp_mat) == data.expect.inp_match_act


def test_percent_error(name, data, exception):
    # print('')
    # print(data.name)
    inp_act = np.array(data.input.act).reshape(-1, 1)
    inp_exp = np.array(data.input.exp).reshape(-1, 1)

    with exception:
        act = matops.percent_error(inp_act, inp_exp)

        exp = data.expect if data.expect != "np.inf" else np.inf

        # print(exp)
        # print(act)

        assert act >= 0.0
        assert isclose(act, exp, abs_tol=0.000_01)


def test_reshape(name, data, exception):
    inp_mat = np.array(data.input.mat)
    inp_newshape = (
        tuple(data.input.newshape.value)
        if data.input.newshape.as_tuple
        else data.input.newshape.value
    )

    with exception:
        act = matops.reshape(inp_mat, inp_newshape)

        exp = np.array(data.expect.mat)

        assert np.allclose(act, exp)


def test_set_row_diagonal_to_one(name, data, exception):
    inp_inplace = data.input.inplace
    inp_mat = np.array(data.input.mat)
    inp_row = data.input.row

    with exception:
        act = matops.set_row_diagonal_to_one(inp_mat, inp_row, inp_inplace)

        exp = np.array(data.expect.mat)

        # print('')
        # print(data.name)
        # print(inp)
        # print(exp)
        # print(act)

        assert np.allclose(act, exp)
        # make sure inp matrix was not mutated
        assert np.allclose(act, inp_mat) == data.expect.inp_match_act


def test_set_rows_below_to_zero(name, data, exception):
    inp_inplace = data.input.inplace
    inp_mat = np.array(data.input.mat)
    inp_row = data.input.source_row

    with exception:
        act = matops.set_rows_below_to_zero(inp_mat, inp_row, inp_inplace)

        exp = np.array(data.expect.mat)

        assert np.allclose(act, exp)
        # make sure inp matrix was not mutated
        assert np.allclose(act, inp_mat) == data.expect.inp_match_act


def test_subtract(name, data, exception):
    inp_mat1 = np.array(data.input.mat1, dtype=float)
    inp_mat2 = np.array(data.input.mat2, dtype=float)

    with exception:
        act = matops.subtract(inp_mat1, inp_mat2)

        exp = np.array(data.expect.mat, dtype=float)

        assert matops.almost_equal(act, exp)


def test_subtract_scalar_row_from_row(name, data, exception):
    inp_inplace = data.input.inplace
    inp_mat = np.array(data.input.mat)
    inp_aff_row = data.input.affect_row
    inp_src_row = data.input.source_row

    with exception:
        act = matops.subtract_scalar_row_from_row(
            inp_mat, inp_src_row, inp_aff_row, inp_inplace
        )

        exp = np.array(data.expect.mat)

        # print('')
        # print(inp)
        # print(exp)
        # print(act)

        assert np.allclose(act, exp)
        # make sure inp matrix was not mutated
        assert np.allclose(act, inp_mat) == data.expect.inp_match_act


def test_swap_largest_pivot_to_top(name, data, exception):
    inp_inplace = data.input.inplace
    inp_mat = np.array(data.input.mat)
    inp_pivot = data.input.pivot

    with exception:
        act = matops.swap_largest_pivot_to_top(inp_mat, inp_pivot, inp_inplace)

        exp = np.array(data.expect.mat)

        # print('')
        # print(inp)
        # print(exp)
        # print(act)

        assert np.allclose(act, exp)
        # make sure inp matrix was not mutated
        assert np.allclose(act, inp_mat) == data.expect.inp_match_act


def test_to_reduced_row_echelon(name, data, exception):
    inp = np.array(data.input)

    with exception:
        act = matops.to_reduced_row_echelon(inp)

        exp = np.array(data.expect)

        # print('')
        # print(inp)
        # print(exp)
        # print(act)

        assert np.allclose(act, exp)


def test_two_norm_of_error(name, data, exception):
    inpA = np.array(data.input.matA)
    inpb = np.array(data.input.matb)
    inpx = np.array(data.input.matx)

    with exception:
        act = matops.two_norm_of_error(inpA, inpb, inpx)

        exp = data.expect

        assert isclose(act, exp, abs_tol=0.000_01)
