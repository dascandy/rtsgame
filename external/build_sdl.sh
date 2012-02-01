#!/bin/bash

cd external/SDL-*
./configure
make
mkdir -p ../../lib
cp build/.libs/libSDL.so ../../lib
cd ../../

