export CC=/usr/bin/i586-mingw32msvc-cc
export CXX=/usr/bin/i586-mingw32msvc-c++
export AR=/usr/bin/i586-mingw32msvc-ar

export RML="$(pwd)"
export RML_PCRE_INCLUDE="$(pwd)/../../deps-win32/include"
export RML_PCRE_LIB="$(pwd)/../../deps-win32/lib"

export MINGW="true"

./compile_morph.sh
./compile_and_check_graphan.sh Russian
