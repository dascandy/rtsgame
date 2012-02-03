#!/bin/bash

cd external/SDL-*
./configure --prefix=$PWD/../../build
make
make install
touch ../sdl_built
cd ../../

