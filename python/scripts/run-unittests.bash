#!/usr/bin/env bash

coverage run --source=src -m pytest -v --durations=0 --withslow &&
coverage report --fail-under=85 -m &&
coverage xml
