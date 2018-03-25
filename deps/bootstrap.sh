#!/bin/bash

path=$(pwd)
git submodule init
git submodule update --recursive pcre
cd ./pcre && cmake . && make -j
cd $path
if [[ $(ldconfig -p | grep libboost) ]]; then
    echo "System boost found"
else
    echo "System boost not found, will compile from srcs"
    git submodule update --recursive boost
    cd boost && git submodule update --init && git checkout boost-1.66.0
    ./bootstrap.sh && ./b2 cxxflags=-fPIC cflags=-fPIC install --link=static --runtime-link=static --prefix=./build/
fi
cd $path
cd aot && ./build-linux.sh

