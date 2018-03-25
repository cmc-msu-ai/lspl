#!/bin/bash

cd ./pcre && git submodule update && cmake . && make -j
cd ..
if [[ $(ldconfig -p | grep libboost) ]]; then
    echo "System boost found"
else
    echo "System boost not found, will compile from srcs"
    cd boost && git submodule update && ./b2 cxxflags=-fPIC cflags=-fPIC install --link=static --runtime-link=static --prefix=./build/
fi
cd ..


