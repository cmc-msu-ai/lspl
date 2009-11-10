#!/bin/sh

rm -Rf build

mkdir build && cd build && cmake -DCMAKE_SYSTEM_NAME=Linux .. -DCMAKE_BUILD_TYPE=Debug ..
