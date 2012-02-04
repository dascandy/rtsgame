#!/bin/bash

cd external/devil-*
./configure --prefix=$PWD/../../build
make
make install
touch ../devil_built
cd ../../

