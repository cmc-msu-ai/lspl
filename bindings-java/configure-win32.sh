#!/bin/sh

rm -Rf build-win32

mkdir build-win32 && cd build-win32 && cmake -DCMAKE_TOOLCHAIN_FILE=../common/Toolchain-mingw32.cmake -DCMAKE_BUILD_TYPE=Release ..
