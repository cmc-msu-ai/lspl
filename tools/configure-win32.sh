#!/bin/sh

export CC=i586-mingw32msvc-gcc
export CXX=i586-mingw32msvc-g++
export LD=i586-mingw32msvc-ld
export AR=i586-mingw32msvc-ar
export AS=i586-mingw32msvc-as
export NM=i586-mingw32msvc-nm
export STRIP=i586-mingw32msvc-strip
export RANLIB=i586-mingw32msvc-ranlib
export DLLTOOL=i586-mingw32msvc-dlltool
export OBJDUMP=i586-mingw32msvc-objdump
export RESCOMP=i586-mingw32msvc-windres

rm -Rf build-win32

mkdir build-win32 && cd build-win32 && cmake -DCMAKE_SYSTEM_NAME=Windows ..
