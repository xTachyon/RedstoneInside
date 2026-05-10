#!/usr/bin/env bash
set -ex

cd /x

rm -rf build
# cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G Ninja
# cmake --build build


ls -lh /cmake/cmake-3.8.2-Linux-x86_64
cmake --version
mkdir build
cd build
cmake .. -DBOOST_INCLUDEDIR=/b/boost/include -DBOOST_LIBRARYDIR=/b/boost/lib -DJSON_REPO_DIR=/json/json
make -j$(nproc)