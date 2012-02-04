#!/bin/bash

cd external/glew-*
GLEW_DEST=$PWD/../../build make 
GLEW_DEST=$PWD/../../build make install
touch ../glew_built
cd ../../

