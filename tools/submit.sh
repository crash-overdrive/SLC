#!/usr/bin/env bash

ln -s tools/Makefile.submit Makefile
zip -FSr "$1".zip . -x "*build*" "*.git*"
/u/cs_build/bin/marmoset --username=-f5fei-c39gao-spratap- submit CS444 ${1}Code "$1".zip
rm Makefile "$1".zip
