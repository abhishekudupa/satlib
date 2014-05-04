#!/bin/bash

NUMCORES=`grep -c '^processor' /proc/cpuinfo`
rm -rf build
autoconf
./configure
python scripts/mk_make.py
cd build
make -j 8
cp libz3.so ../

