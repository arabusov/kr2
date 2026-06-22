#!/bin/sh
echo "*** Unit tests ***"
./run_utests
echo "*** Positive tests ***"
N_TESTS=0
N_FAIL=0
for f in $(find tests/ -type f); do
    let N_TESTS=${N_TESTS}+1
    cpp $f | ./scan -q
    if [ ! $? -eq 0 ]; then
        echo $f
        let N_FAIL=${N_FAIL}+1
    fi
done
echo "N_tests  = ${N_TESTS}"
echo "N_failed = ${N_FAIL}"
