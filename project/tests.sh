#!/bin/bash

echo "Checking test.txt"

expected="$(sha512sum ./testfiles/test.txt)"
myoutput="$(./sha512 ./testfiles/test.txt)"

echo "Expected: " $expected
echo "Output  : " $myoutput

if [[ $myoutput == $expected ]]; then
    echo "Passed the test"
else
    echo "Failed the test"
fi

echo "Checking empty.txt"

expected="$(sha512sum ./testfiles/empty.txt)"
myoutput="$(./sha512 ./testfiles/empty.txt)"

echo "Expected: " $expected
echo "Output  : " $myoutput

if [[ $myoutput == $expected ]]; then
    echo "Passed the test"
else
    echo "Failed the test"
fi