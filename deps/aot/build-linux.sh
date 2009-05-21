export RML="$(pwd)"

if [ -f /usr/include/pcrecpp.h ]; then
	export RML_PCRE_INCLUDE=/usr/include/
	export RML_PCRE_LIB=/usr/lib
elif [ -f /usr/local/include/pcrecpp.h ]; then
	export RML_PCRE_INCLUDE=/usr/local/include/
	export RML_PCRE_LIB=/usr/local/lib
else
	echo "Error: Cann't find PCRE headers"
	exit 0
fi

./compile_morph.sh
./compile_and_check_graphan.sh Russian
