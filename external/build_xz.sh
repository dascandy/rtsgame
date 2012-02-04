#!/bin/bash

cd external/xz-*
./configure --prefix=$PWD/../../build
make
make install
touch ../xz_built
cd ../../

