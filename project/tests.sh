#!/bin/bash

echo "Checking test.txt"

expected="$(sha512sum test.txt)"
myoutput="$(./sha512 test.txt)"

echo "Expected: " $expected
echo "Output  : " $myoutput

if [[ $myoutput == $expected ]]; then
    echo "Passed the test"
else
    echo "Failed the test"
fi

echo "Checking empty.txt"

expected="$(sha512sum empty.txt)"
myoutput="$(./sha512 empty.txt)"

echo "Expected: " $expected
echo "Output  : " $myoutput

if [[ $myoutput == $expected ]]; then
    echo "Passed the test"
else
    echo "Failed the test"
fi