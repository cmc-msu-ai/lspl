export RML="$(pwd)"
export RML_PCRE_INCLUDE=/usr/include/
export RML_PCRE_LIB=/usr/lib

./compile_morph.sh
./compile_and_check_graphan.sh Russian
