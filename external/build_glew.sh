#!/bin/bash

cd external/glew-*
make
mkdir ../../lib
cp lib/libGLEW.so ../../lib
cd ../../

