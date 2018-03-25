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
    cd boost && ./b2 cxxflags=-fPIC cflags=-fPIC install --link=static --runtime-link=static --prefix=./build/
fi
cd $path
cd aot && ./build-linux.sh

