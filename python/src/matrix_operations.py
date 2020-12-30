
from copy import deepcopy as pydeepcopy
import numpy as np
from os import path, scandir
from random import uniform
from tempfile import mkdtemp


def almost_equal(mat1, mat2):
    return np.allclose(mat1, mat2)

def create_augmented(matA, matb):
    """
    A new matrix is returned and inputs are not modified.
    """
    _, colsb = matb.shape
    if colsb > 1:
        raise ValueError('Matrix b must have only 1 column, here has %i' % colsb)
    return np.hstack((matA, matb))

def create_random(size, single_column=False):
    if size < 2 or size > 500:
        raise ValueError('size input must be between 2..500, here is %i' % size)

    columns = 1 if single_column else size

    # TODO use numpy.random to generate this matrix
    # https://stackoverflow.com/a/8489498
    mat = np.zeros((size, columns))
    for i in range(size):
        mat[i,:] = [uniform(-20,20) for c in range(columns)]
    
    return mat

def create_random_diagonal_dominate(size):
    ABRITARY_SCALAR = 2
    mat = create_random(size)
    sums = [np.sum(np.fabs(mat[r,:])) * ABRITARY_SCALAR for r in range(size)]
    for i in range(size):
        mat[i,i] = sums[i]

    return mat

def create_zeros(rows: int=1, columns: int=0):
    if not rows or rows < 1:
        rows = 1
    if columns and columns < 1:
        raise ValueError('Columns is {}, but expected at least 1.'.format(columns))

    if not columns:
        return np.zeros(rows)
    else:
        return np.zeros((rows, columns))


def deepcopy(inp, do_it=True):
    """
    do_it: (True) actually perform deepcopy; otherwise, return input. For cases
            when some other higher function might have already performed a
            deepcopy.
    """

    if do_it:
        return pydeepcopy(inp)
    return inp

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
        row = np.take(matrix[r,:], indices)
        expect = np.zeros(indices.shape[0], dtype=float)
        if not np.allclose(row, expect):
            return False

    return True

def is_augmented(matrix):
    rows, cols = matrix.shape
    return cols == rows + 1

def is_singular(matrix):
    if not (is_square(matrix) ^ is_augmented(matrix)):
        return False

    if is_augmented(matrix):
        temp = matrix[:,:matrix.shape[1] - 1]
    else:
        temp = matrix

    sign, logdet = np.linalg.slogdet(temp)
    # print('sign={};logdet={}'.format(sign, logdet))
    return sign == 0 and logdet == -np.inf


def is_square(matrix):
    rows, cols = matrix.shape
    return rows > 0 and rows == cols


def load_files(directory):
    with scandir(directory) as files:
        for file in files:
            fname, _ = path.splitext(path.basename(file))
            with open(file, 'r') as fp:
                if fname == 'A':
                    A = np.loadtxt(fp, delimiter=' ')
                elif fname == 'b':
                    b = np.reshape(np.loadtxt(fp, delimiter=' '), (-1, 1))
                elif fname == 'soln':
                    soln = np.reshape(np.loadtxt(fp, delimiter=' '), (-1, 1))

    return (A, b, soln)

def multiply(a, b):
    return np.matmul(a,b)

def multiply_row_by_scalar(matrix, row, scalar, inplace=True):
    """
    inplace: (True) input matrix is modified as part of the operation;
            otherwise, input matrix is copied.
    """

    if row >= matrix.shape[0]:
        return matrix

    ret = deepcopy(matrix, (not inplace))
    # print('multiplying row %i by %.6f' % (row, scalar))
    ret[row,:] = (ret[row,:] * scalar) + 0.0
    return ret


def print_pretty(matrix):
    # TODO do we really need this?
    # https://stackoverflow.com/a/1988024
    raise NotImplementedError

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

    diag = ret[row,row]
    ret = multiply_row_by_scalar(ret, row, (1.0/diag))

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
    lead = ret[aff_row,src_row]
    ret[aff_row,src_row:] = ret[aff_row,src_row:] - (ret[src_row,src_row:] * lead)
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

    cols = np.fabs(np.reshape(ret[pivot:,pivot], (-1,1)))
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
        raise ValueError('Either input MatrixA, MatrixB, or solution matrix MatrixX is None.')

    # reshape matx from 1xN to be Nx1 if need be
    x = matx.reshape((matx.shape[0],1)) if len(matx.shape) == 1 else matx

    ax = multiply(matA, x)
    bax = subtract(matb, ax)
    ret = np.sum(bax[:,0] ** 2)
    ret = sqrt(ret)

    return ret

def write_files(*args):
    tdir = mkdtemp(prefix='makemat-')
    for m, name in args:
        np.savetxt(path.join(tdir, name), m, fmt='%.6f', delimiter=' ')

    return tdir
