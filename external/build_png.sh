#!/bin/bash

cd external/libpng-*
./configure
make
mkdir -p ../../lib
cp .libs/libpng15.so ../../lib/libPNG.so
cd ../../

