#!/usr/bin/env bash

echo "Clean makemat" &&
rm -rf /tmp/makemat &&

echo "Make 3" &&
python -m src make 3 > /dev/null &&
echo "Check 3" &&
python -m src solve /tmp/makemat --check > /dev/null &&

echo "Make 10" &&
python -m src make 10 > /dev/null &&
echo "Check 10" &&
python -m src solve /tmp/makemat --check > /dev/null &&

echo "Make 500" &&
python -m src make 500 > /dev/null &&
echo "Check 500" &&
python -m src solve /tmp/makemat --check > /dev/null