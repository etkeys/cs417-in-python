#!/usr/bin/env bash

coverage run --source=src -m pytest -v --durations=0 --withslow --withsubprocess &&
coverage report -m
