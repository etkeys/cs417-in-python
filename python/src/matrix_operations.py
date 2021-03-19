from copy import deepcopy as pydeepcopy
from typing import Type
import numpy as np
from os import path, scandir, makedirs
from random import uniform
from tempfile import gettempdir, tempdir


def add(a, b):
    return a + b


def almost_equal(mat1, mat2):
    return np.allclose(mat1, mat2, atol=0.000_01)


def column_max(matrix, index):
    if not is_matrix(matrix):
        _raise_not_a_matrix()
    if is_hvector(matrix):
        return np.max(matrix)
    elif is_vvector(matrix):
        return np.max(matrix[:, 0])
    else:
        if index < 0:
            raise IndexError(f"Index {index} is out of bounds for axis 1.")
        return np.max(matrix[:, index])


def count_columns(matrix):
    if not is_matrix(matrix):
        _raise_not_a_matrix()
    if is_hvector(matrix):
        return len(matrix)
    return matrix.shape[1]


def count_rows(matrix):
    if not is_matrix(matrix):
        _raise_not_a_matrix()
    return 1 if is_hvector(matrix) else matrix.shape[0]


def create_augmented(matA, matb):
    """
    A new matrix is returned and inputs are not modified.
    """
    if not is_vector(matb):
        raise ValueError("Matrix b must be a vector.")
    return (
        np.hstack((matA, matb))
        if is_vvector(matb)
        else np.hstack((matA, reshape(matb, (-1, 1))))
    )


def create_based_on_diagonal_terms(matrix):
    if not is_matrix(matrix):
        _raise_not_a_matrix()
    if not is_square(matrix):
        raise ValueError("Input matrix must be square.")

    diag = np.diag(matrix)
    size = matrix.shape[0]
    ret = create_zeros(size, size)
    np.fill_diagonal(ret, diag)

    return ret


def create_based_on_l_component(matrix):
    if not is_square(matrix):
        raise ValueError("Input matrix must be square")
    return np.tril(matrix)


def create_based_on_non_diagonal_terms(matrix):
    if not is_matrix(matrix):
        _raise_not_a_matrix()
    if not is_square(matrix):
        raise ValueError("Input matrix must be square")

    ret = deepcopy(matrix)
    np.fill_diagonal(ret, 0.0)

    return ret


def create_based_on_u_component(matrix, diag_zero: bool = True):
    if not is_square(matrix):
        raise ValueError("Input matrix must be square")
    offset = 1 if diag_zero else 0
    return np.triu(matrix, offset)


def _create_fill_with_constant(const_val, rows: int = None, columns: int = None):
    if not rows or rows < 1:
        rows = 1
    if columns and columns < 1:
        raise ValueError("Columns is {}, but expected at least 1.".format(columns))

    inp = (rows, columns) if columns is not None and columns > 0 else rows
    if const_val == 1:
        return np.ones(inp)
    else:
        return np.zeros(inp)


def create_identity(size):
    if not isinstance(size, int):
        raise TypeError("SIZE expected to be type in, but here is %s" % type(size))
    if size < 1:
        raise ValueError("SIZE expected to be greater than 0, here is %i" % size)
    return np.identity(size)


def create_inverted(matrix):
    if not is_matrix(matrix):
        _raise_not_a_matrix()
    if not is_square(matrix) or is_singular(matrix):
        raise ValueError("Only square, non-singular matrices have an inverse")

    # It seems that we don't need to care about creating diagonal only
    # inverse (jacobi) and an entire matrix inverse. This function seems
    # to handle them both.
    ret = np.linalg.inv(matrix)

    return ret


def create_ones(rows: int = None, columns: int = None):
    return _create_fill_with_constant(1, rows, columns)


def create_random(size, single_column=False):
    if size < 2 or size > 500:
        raise ValueError("size input must be between 2..500, here is %i" % size)

    columns = 1 if single_column else size

    b = 20
    a = -b
    rng = np.random.default_rng()
    # from np.random.Generator.random docs..
    # To sample from interval [a, b), b > a; multiply random by (b-a) and add a
    return (b - a) * rng.random((size, columns)) + a


def create_random_diagonal_dominate(size):
    ABRITARY_SCALAR = 2
    mat = create_random(size)
    sums = [np.sum(np.fabs(mat[r, :])) * ABRITARY_SCALAR for r in range(size)]
    for i in range(size):
        mat[i, i] = sums[i]

    return mat


def create_zeros(rows: int = None, columns: int = None):
    return _create_fill_with_constant(0, rows, columns)


def deepcopy(inp, do_it=True):
    """
    do_it: (True) actually perform deepcopy; otherwise, return input. For cases
            when some other higher function might have already performed a
            deepcopy.
    """

    if do_it:
        return pydeepcopy(inp)
    return inp


def is_in_crout_l_form(matrix):
    if not is_square(matrix):
        return False

    # For every row, does every column to the right of diagonal have a value of 0.0?
    size = count_rows(matrix)
    temp = matrix[np.triu_indices(size, k=1)]
    zeros = create_zeros(temp.size)

    return np.allclose(temp, zeros, atol=0.00001)


def is_in_reduced_row_echelon(matrix):
    rows, columns = matrix.shape
    if rows > columns:
        return False

    expect = np.ones((rows,), dtype=float)
    if not np.allclose(matrix.diagonal(), expect):
        return False

    for r in range(rows):
        if r == 0:
            continue
        indices = np.arange(r)
        row = np.take(matrix[r, :], indices)
        expect = np.zeros(indices.shape[0], dtype=float)
        if not np.allclose(row, expect):
            return False

    return True


def is_augmented(matrix):
    rows, cols = matrix.shape
    return cols == rows + 1


def is_hvector(matrix):
    if not is_matrix(matrix):
        return False
    if len(matrix.shape) == 1:
        rows = 1
        cols = matrix.shape[0]
    else:
        rows, cols = matrix.shape
    return rows == 1 and cols >= rows


def is_matrix(matrix):
    # Have to confirm if matrix by checking shape length because "None" can
    # be considered a Matrix (for some reason)
    if isinstance(matrix, np.ndarray) and len(matrix.shape) > 0:
        return True
    return False


def is_singular(matrix, allow_augmented=True):
    is_aug = is_augmented(matrix)
    is_sqr = is_square(matrix)
    if not (is_sqr ^ is_aug):
        return False

    temp = matrix[:, : matrix.shape[1] - 1] if is_aug else matrix

    sign, logdet = np.linalg.slogdet(temp)
    # print('sign={};logdet={}'.format(sign, logdet))
    return sign == 0 and logdet == -np.inf


def is_square(matrix):
    if not is_matrix(matrix):
        return False
    if is_vector(matrix) and len(matrix.shape) < 2:
        return matrix.shape[0] == 1
    rows, cols = matrix.shape
    return rows > 0 and rows == cols


def is_vector(matrix):
    return is_hvector(matrix) or is_vvector(matrix)


def is_vvector(matrix):
    if not is_matrix(matrix):
        return False
    if is_hvector(matrix):
        return matrix.shape[0] == 1
    rows, cols = matrix.shape
    return cols == 1 and rows >= cols


def load_files(directory, do_reshape=None, omega_as_matrix: bool = False):
    """
    Load data from text files ine given directory

    Parameters:
        directory (str): The path, relative to the current directory, for which to
            seach for files
        do_reshape (bool): False, do not perform implicit reshaping based on file name;
            otherwise, automatic reshaping is performed.

    Returns:
    (Matrix A, Matrix b, Matrix Soln)
    """

    def _call_load(fp):
        return np.loadtxt(fp, dtype=float, delimiter=" ")

    ret = {}
    do_reshape = True if do_reshape is None else do_reshape
    with scandir(directory) as files:
        for file in files:
            fname, _ = path.splitext(path.basename(file))
            with open(file, "r") as fp:
                if fname == "A":
                    obj = _call_load(fp)
                    fname = "mat" + fname
                elif fname == "b":
                    obj = _call_load(fp)
                    if do_reshape:
                        obj = reshape(obj, (-1, 1))
                    fname = "mat" + fname
                elif fname == "omega":
                    obj = _call_load(fp)
                    if not omega_as_matrix:
                        obj = obj.item(0)
                elif fname == "soln":
                    obj = _call_load(fp)
                    if do_reshape:
                        obj = reshape(obj, (-1, 1))
                    fname = "mat" + fname

                ret[fname] = obj

    return ret


def multiply(a, b):
    if not is_matrix(a):
        _raise_not_a_matrix()
    if is_matrix(b):
        return np.matmul(a, b)
    elif isinstance(b, (int, float)):
        return a * b
    else:
        raise TypeError(
            "Argument 2 expected to be matrix or scalar, but here is %s"
            % type(b).__name__
        )


def multiply_row_by_scalar(matrix, row, scalar, inplace=True):
    """
    inplace: (True) input matrix is modified as part of the operation;
            otherwise, input matrix is copied.
    """

    if row >= matrix.shape[0]:
        return matrix

    ret = deepcopy(matrix, (not inplace))
    # print('multiplying row %i by %.6f' % (row, scalar))
    ret[row, :] = (ret[row, :] * scalar) + 0.0
    return ret


def reshape(matrix, newshape=None):
    if not is_matrix(matrix):
        _raise_not_a_matrix()
    if newshape is None:
        newshape = -1
    elif isinstance(newshape, int):
        if not (newshape > 0 or newshape == -1):
            raise ValueError("Argument newshape value must be -1 or greater than 0.")
        # If newshape is just 1, caller only wants a single row, spare them the pain
        # and just give them one row.
        if newshape == 1:
            newshape = -1
    elif isinstance(newshape, tuple):
        if not all([isinstance(v, int) for v in newshape]):
            raise TypeError("Argument newshape must be of type int or tuple of ints.")
        if not all([v > 0 or v == -1 for v in newshape]):
            raise ValueError("Argument newshape values must be -1 or greater than 0.")
    else:
        raise TypeError("Argument newshape must be of type int or tuple of ints.")
    return matrix.reshape(newshape)


def scalar_sum(matrix):
    if not is_matrix(matrix):
        _raise_not_a_matrix()
    return np.sum(matrix)


def set_rows_below_to_zero(matrix, base_row, inplace=True):
    """
    inplace: (True) input matrix is modified as part of the operation;
            otherwise, input matrix is copied.
    """

    if base_row + 1 >= matrix.shape[0]:
        return matrix

    ret = deepcopy(matrix, (not inplace))
    # print('set rows below %i to 0 [in]' % base_row)
    # print(ret)

    for row in range(base_row + 1, ret.shape[0]):
        ret = subtract_scalar_row_from_row(ret, base_row, row)

    # print('set rows below %i to 0 [out]' % base_row)
    # print(ret)
    return ret


def set_row_diagonal_to_one(matrix, row, inplace=True):
    """
    inplace: (True) input matrix is modified as part of the operation;
            otherwise, input matrix is copied.
    """

    if row >= matrix.shape[0]:
        return matrix

    ret = deepcopy(matrix, (not inplace))
    # print('set row %i diagonal to 1 [in]' % row)
    # print(ret)

    diag = ret[row, row]
    ret = multiply_row_by_scalar(ret, row, (1.0 / diag))

    if ret[row, row] < 0:
        ret = multiply_row_by_scalar(ret, row, -1.0)

    # print('set row %i diagonal to 1 [out]' % row)
    # print(ret)
    return ret


def subtract(mat1, mat2):
    return np.subtract(mat1, mat2)


def subtract_scalar_row_from_row(matrix, src_row, aff_row, inplace=True):
    """
    inplace: (True) input matrix is modified as part of the operation;
            otherwise, input matrix is copied.
    """

    if aff_row >= matrix.shape[0]:
        return matrix

    ret = deepcopy(matrix, (not inplace))
    # print('subtract scalar row of %i from row %i [in]' % (src_row, aff_row))
    # print(ret)

    # what will it take to make the row leading value 0?
    # 1. take the leading value,
    # 2. multiply source row by leading value to get scalar row
    # 3. subtract scalar row from current row,
    # 4. multiply source row by inverse of leading value.
    lead = ret[aff_row, src_row]
    ret[aff_row, src_row:] = ret[aff_row, src_row:] - (ret[src_row, src_row:] * lead)
    # print('subtract scalar row of %i from row %i [out]' % (src_row, aff_row))
    # print(ret)
    return ret


def swap_largest_pivot_to_top(matrix, pivot, inplace=True):
    """
    inplace: (True) input matrix is modified as part of the operation;
            otherwise, input matrix is copied.
    """

    if pivot >= matrix.shape[0]:
        return matrix

    ret = deepcopy(matrix, (not inplace))
    # print('swap rows [in]')
    # print(ret)

    cols = np.fabs(np.reshape(ret[pivot:, pivot], (-1, 1)))
    big, _ = np.where(cols == np.max(cols))

    swap = int(big) + pivot
    if swap > pivot:
        # print('swaping row %i with %i' % (pivot, swap))
        ret[[pivot, swap]] = ret[[swap, pivot]]

    # print('swap rows [out]')
    # print(ret)
    return ret


def to_reduced_row_echelon(matrix):
    """
    This implementation does not yield a true reduced row echelon form matrix.
    To get the actual result vector, you'll have to calculate using back
    solving.
    """
    if is_in_reduced_row_echelon(matrix):
        return matrix

    # deepcopy because we don't want to muck with the original
    ret = deepcopy(matrix)
    for r in range(matrix.shape[0]):
        ret = swap_largest_pivot_to_top(ret, r)
        ret = set_row_diagonal_to_one(ret, r)
        ret = set_rows_below_to_zero(ret, r)

    if not is_in_reduced_row_echelon(ret):
        # print(ret)
        raise RuntimeError("Matrix not in reduced row echelon form when expected.")

    return ret


def two_norm_of_error(matA, matb, matx):
    if matA is None or matb is None or matx is None:
        raise ValueError(
            "Either input MatrixA, MatrixB, or solution matrix MatrixX is None."
        )

    x = matx if is_vvector(matx) else reshape(matx, (-1, 1))
    ret = np.linalg.norm(subtract(multiply(matA, x), matb))
    return ret


def percent_error(vec_actual, vec_expected):
    top = np.sum(np.fabs(subtract(vec_expected, vec_actual)))
    bot = np.sum(vec_expected)
    try:
        with np.errstate(divide="ignore"):
            ret = np.fabs(top / bot)
    except ZeroDivisionError:
        ret = np.inf

    return ret


def write_files(items_dict, out_dir):
    out_dir = out_dir if path.isabs(out_dir) else path.join(gettempdir(), out_dir)
    makedirs(out_dir, exist_ok=True)

    for k, v in items_dict.items():
        fname = k[3:] if k.startswith("mat") else k
        fname = f"{fname}.def"
        with open(path.join(out_dir, fname), "wt") as fp:
            np.savetxt(fp, v, fmt="%.6f", delimiter=" ")

    return out_dir


def _raise_not_a_matrix():
    raise TypeError("Argument matrix is None or not a matrix.")
