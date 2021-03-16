#!/usr/bin/env bash

export TIMEFORMAT="Elapsed time (seconds): %E"

solvers=('gaussian' 'ludecomposition' 'jacobi' 'gaussseidel')
omegas=('0.5' '0.7' '1.0' '1.2' '1.5')

function run_size {
    size=$1

    echo "======================================"
    rm -rf /tmp/makemat &&

    echo "Make $size" &&
    python -m src make $size > /dev/null &&

    (
        for solver in "${solvers[@]}"; do
            (
                echo "---- Check $solver ----" &&
                time python -m src solve --quite /tmp/makemat "$solver" --check 2>&1
            ) || exit 1
        done
    ) &&
    (
        for omega in "${omegas[@]}"; do
            (
                echo "---- Check SOR ($omega) ----" &&
                time python -m src solve --quite /tmp/makemat sor --check --omega "$omega" 2>&1
            ) || exit 1
        done
    )
}

run_size 3 &&
run_size 10 &&
run_size 500
