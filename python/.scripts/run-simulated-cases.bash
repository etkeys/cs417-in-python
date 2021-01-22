#!/usr/bin/env bash

solvers=('gaussian' 'ludecomposition')

function run_size {
    size=$1

    echo "Clean makemat" &&
    rm -rf /tmp/makemat &&

    echo "Make $size" &&
    python -m src make $size > /dev/null &&

    (
        for solver in "${solvers[@]}"; do
            (
                echo "Check $solver" &&
                python -m src solve /tmp/makemat "$solver" --check > /dev/null
            ) || exit 1
        done
    )
}

run_size 3 &&
run_size 10 &&
run_size 500
