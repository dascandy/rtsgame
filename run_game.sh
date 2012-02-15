#!/bin/bash

make build/bin/game

LD_LIBRARY_PATH=build/lib:build/lib64 build/bin/game

