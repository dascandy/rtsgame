#!/bin/bash

cd external/libogg-*
./configure --prefix=$PWD/../../build
make
make install
touch ../ogg_built
cd ../../

