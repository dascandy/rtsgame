#!/bin/bash

cd external/libvorbis-*
./configure --prefix=$PWD/../../build
make
make install
touch ../vorbis_built
cd ../../

