
#!/usr/bin/env bash

function run_app {
    echo "" &&
    echo "Check Assignment $1" &&
    python -m src solve "assignments/$2" "$3" --check --quite
}

run_app "1" "1" "gaussian" &&
run_app "2" "2" "ludecomposition" &&
run_app "3 - Jacobi" "3" "jacobi" &&
run_app "3 - Gauss-Seidel" "3" "gaussseidel" &&
run_app "3 - SOR" "3" "sor"
