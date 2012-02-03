#!/bin/bash

cd external/bullet-*
cmake -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX="$PWD/../../build" -DCMAKE_INSTALL_RPATH="$PWD/../../build" .
make
make install
touch ../bullet_built
cd ../../

