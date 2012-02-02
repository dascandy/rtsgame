#!/bin/bash

cd external/libogg
./configure
make
mkdir -p ../../lib
cp src/.libs/libogg.so ../../lib
cd ../../

