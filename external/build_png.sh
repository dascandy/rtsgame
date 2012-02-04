#!/bin/bash

cd external/libpng-*
./configure --prefix=$PWD/../../build
make
make install
touch ../png_built
cd ../../

