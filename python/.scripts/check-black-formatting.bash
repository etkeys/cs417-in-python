
#!/usr/bin/env bash

BASEDIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"


function run_black {
    black --target-version py36 --check "$1"
}

cd "$BASEDIR/.." &&
run_black "src" &&
run_black "tests"