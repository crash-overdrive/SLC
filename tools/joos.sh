#!/usr/bin/env bash

if [[ ! -d output ]]; then
  echo "missing class directory named output"
  exit 1
fi

cp -u ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/stdlib/5.0/runtime.s output
make -s -f ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Makefile.joos
./main
echo "Exit code:" $?
