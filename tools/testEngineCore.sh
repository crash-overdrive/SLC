#!/usr/bin/env bash

#S1 is the location of the executable (joosc)
#S2 is the location of the executable (./joos)
#$3 is the location of the tests (testdata/java/a5)

i=1

for dir in $(ls $3);do
echo "---------------------------------------------------------"
echo Test Case $i - $dir
i=$((i+1))
$1 --std-lib5 $(ls $3$dir) && $2
if [ $? -ne 123 ]
then
echo "Return code: " $?
fi
done
