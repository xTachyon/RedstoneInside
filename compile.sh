#!/usr/bin/env bash
set -ex

cd /x


export CC=clang
export CXX=clang++

rm -rf build/CMakeCache.txt
# cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G Ninja
# cmake --build build

cmake --version
mkdir -p build
cd build
cmake .. -DBOOST_INCLUDEDIR=/libs/boost/include -DBOOST_LIBRARYDIR=/libs/boost/lib -DJSON_REPO_DIR=/libs/json -DCMAKE_BUILD_TYPE=Debug -G Ninja
cmake --build .
