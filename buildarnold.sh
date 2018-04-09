#!/bin/bash
# A bash script to compile and run C code on RPi
# errorTeam by Lars

# uncomment 1 of the following to choose branch:
# git checkout master
# git checkout development

#git pull
g++ arnold.cpp BrickPi3.cpp -o arnoldcompiled
./arnoldcompiled
