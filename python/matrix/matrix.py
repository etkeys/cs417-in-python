
from math import fabs
import numpy as np
from os import path
from random import uniform
from tempfile import mkdtemp

def create_random(size, single_column=False):
    if size < 2 or size > 500:
        raise ValueError('size input must be between 2..500, here is %i' % size)

    columns = 1 if single_column else size

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

def multiply(a, b):
    return np.matmul(a,b)

def print_pretty(matrix):
    # TODO do we really need this?
    raise NotImplementedError

def write_files(*args):
    tdir = mkdtemp(prefix='makemat-')
    for m, name in args:
        np.savetxt(path.join(tdir, name), m, fmt='%.6f', delimiter=' ')

    return tdir
