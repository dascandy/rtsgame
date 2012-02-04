#!/bin/bash

cd external/glm-*
cp -R glm/ ../../build/include/
touch ../glm_built
cd ../../

