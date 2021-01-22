from copy import deepcopy as pydeepcopy
import numpy as np
from os import path, scandir, makedirs
from random import uniform
from tempfile import gettempdir, tempdir


def almost_equal(mat1, mat2):
    return np.allclose(mat1, mat2, atol=0.000_01)


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


def create_identity(size):
    if not isinstance(size, int):
        raise TypeError("SIZE expected to be type in, but here is %s" % type(size))
    if size < 1:
        raise ValueError("SIZE expected to be greater than 0, here is %i" % size)
    return np.identity(size)


def create_random(size, single_column=False):
    if size < 2 or size > 500:
        raise ValueError("size input must be between 2..500, here is %i" % size)

    columns = 1 if single_column else size

    # TODO use numpy.random to generate this matrix
    # https://stackoverflow.com/a/8489498
    mat = np.zeros((size, columns))
    for i in range(size):
        mat[i, :] = [uniform(-20, 20) for c in range(columns)]

    return mat


def create_random_diagonal_dominate(size):
    ABRITARY_SCALAR = 2
    mat = create_random(size)
    sums = [np.sum(np.fabs(mat[r, :])) * ABRITARY_SCALAR for r in range(size)]
    for i in range(size):
        mat[i, i] = sums[i]

    return mat


def create_zeros(rows: int = None, columns: int = None):
    if not rows or rows < 1:
        rows = 1
    if columns and columns < 1:
        raise ValueError("Columns is {}, but expected at least 1.".format(columns))

    inp = (rows, columns) if columns is not None and columns > 0 else rows
    return np.zeros(inp)


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
    return len(matrix.shape) == 1 or matrix.shape[0] == 1


def is_matrix(matrix):
    # Have to confirm if matrix by checking shape length because "None" can
    # be considered a Matrix (for some reason)
    if isinstance(matrix, np.ndarray) and len(matrix.shape) > 0:
        return True
    return False


def is_singular(matrix):
    if not (is_square(matrix) ^ is_augmented(matrix)):
        return False

    if is_augmented(matrix):
        temp = matrix[:, : matrix.shape[1] - 1]
    else:
        temp = matrix

    sign, logdet = np.linalg.slogdet(temp)
    # print('sign={};logdet={}'.format(sign, logdet))
    return sign == 0 and logdet == -np.inf


def is_square(matrix):
    rows, cols = matrix.shape
    return rows > 0 and rows == cols


def is_vector(matrix):
    return is_vvector(matrix) or is_hvector(matrix)


def is_vvector(matrix):
    return is_matrix(matrix) and not is_hvector(matrix) and matrix.shape[1] == 1


def load_files(directory, do_reshape=None):
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
    do_reshape = True if do_reshape is None else do_reshape
    with scandir(directory) as files:
        for file in files:
            fname, _ = path.splitext(path.basename(file))
            with open(file, "r") as fp:
                if fname == "A":
                    A = np.loadtxt(fp, delimiter=" ")
                elif fname == "b":
                    b = np.loadtxt(fp, delimiter=" ")
                    if do_reshape:
                        b = reshape(b, (-1, 1))
                elif fname == "soln":
                    soln = np.loadtxt(fp, delimiter=" ")
                    if do_reshape:
                        soln = reshape(soln, (-1, 1))

    return (A, b, soln)


def multiply(a, b):
    return np.matmul(a, b)


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


def print_pretty(matrix):
    # TODO do we really need this?
    # https://stackoverflow.com/a/1988024
    raise NotImplementedError


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


# TODO needs tests
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


# TODO Might need to move this into gaussian
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
        # TODO Might need to move these into gaussian
        ret = swap_largest_pivot_to_top(ret, r)
        ret = set_row_diagonal_to_one(ret, r)
        ret = set_rows_below_to_zero(ret, r)

    if not is_in_reduced_row_echelon(ret):
        # print(ret)
        raise RuntimeError("Matrix not in reduced row echelon form when expected.")

    return ret


def two_norm_of_error(matA, matb, matx):
    from math import sqrt

    if matA is None or matb is None or matx is None:
        raise ValueError(
            "Either input MatrixA, MatrixB, or solution matrix MatrixX is None."
        )

    # reshape matx from 1xN to be Nx1 if need be
    x = matx.reshape((matx.shape[0], 1)) if len(matx.shape) == 1 else matx

    ax = multiply(matA, x)
    bax = subtract(matb, ax)
    ret = np.sum(bax[:, 0] ** 2)
    ret = sqrt(ret)

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


def write_files(*args, **kwargs):
    arg_directory = kwargs.get("directory", None)
    out_dir = "makemat" if not arg_directory else arg_directory
    out_dir = out_dir if path.isabs(out_dir) else path.join(gettempdir(), out_dir)
    makedirs(out_dir, exist_ok=True)

    for m, name in args:
        with open(path.join(out_dir, name), "wt") as fp:
            np.savetxt(fp, m, fmt="%.6f", delimiter=" ")

    return out_dir


def _raise_not_a_matrix():
    raise TypeError("Argument matrix is None or not a matrix.")
