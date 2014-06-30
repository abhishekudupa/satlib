#!/bin/bash

NUMCORES=`grep -c '^processor' /proc/cpuinfo`
cd z3-4.3.1
rm -rf build
autoconf
./configure
python scripts/mk_make.py
cd build
make -j $NUMCORES
cp libz3.so ../../

