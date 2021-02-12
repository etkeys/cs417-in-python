#!/usr/bin/env bash

# export TIME="\t%E"
export TIMEFORMAT="Elapsed time (seconds): %E"

solvers=('gaussian' 'ludecomposition' 'jacobi')

function run_size {
    size=$1

    echo "==================="
    echo "Clean makemat" &&
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
    )
}

run_size 3 &&
run_size 10 &&
run_size 500
