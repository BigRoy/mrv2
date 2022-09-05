#!/bin/bash

mkdir build
cd build

cmake .. -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=$PWD/install -D CMAKE_PREFIX_PATH=$PWD/install  -D TLRENDER_BUILD_PROGRAMS=OFF -D TLRENDER_BUILD_EXAMPLES=FALSE -D TLRENDER_BUILD_TESTS=FALSE

cmake --build . -j 4 --config Release

cd ..
