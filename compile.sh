#!/usr/bin/env bash
set -ex

cd /x

rm -rf build/CMakeCache.txt
# cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G Ninja
# cmake --build build

cmake --version
mkdir -p build
cd build
cmake .. -DBOOST_INCLUDEDIR=/b/boost/include -DBOOST_LIBRARYDIR=/b/boost/lib -DJSON_REPO_DIR=/json/json
make -j$(nproc)