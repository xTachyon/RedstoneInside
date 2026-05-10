set -ex

cd /x

rm -rf build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G Ninja
cmake --build build
