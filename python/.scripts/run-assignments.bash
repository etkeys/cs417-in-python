
#!/usr/bin/env bash

echo "Check Assignment 1" &&
python -m src solve assignments/1 gaussian --check > /dev/null &&

echo "Check Assignment 2" &&
python -m src solve assignments/2 ludecomposition --check > /dev/null 