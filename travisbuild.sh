#!/usr/bin/env bash
set -x
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 100 --slave /usr/bin/g++ g++ /usr/bin/g++-6

if [ ! -d "libs/boost/lib/" ]; then
  wget http://kent.dl.sourceforge.net/project/boost/boost/1.62.0/boost_1_62_0.tar.bz2
  tar -xjf boost_1_62_0.tar.bz2
  cd boost_1_62_0/
  sudo ./bootstrap.sh --prefix=../libs/boost/
  sudo ./b2 install -j 8
  cd ..
fi

if [ -f "libs/json/src/json.hpp" ]; then
  cd libs/json/
  git pull
  cd ../..
else
  cd libs/
  git clone https://github.com/nlohmann/json.git
  cd ..
fi

if [ ! -d "libs/openssl-1.0.2g" ]; then
  cd libs/
  wget https://www.openssl.org/source/openssl-1.0.2g.tar.gz
  tar -xzf openssl-1.0.2g.tar.gz
  cd openssl-1.0.2g
  sudo ./config
  sudo make install
  cd ../..
fi

mkdir build
cd build
cmake .. -DBOOST_INCLUDEDIR=libs/boost/include -DBOOST_LIBRARYDIR=libs/boost/lib -DJSON_REPO_DIR=libs/json
make