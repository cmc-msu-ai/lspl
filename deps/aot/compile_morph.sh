#!/bin/sh
# Compiling Dialing/DWDS morphology
# the script is written by Alexey Sokirko 2003, sokirko@yandex.ru



halt() {
	echo  "======================"
	echo  "Error: $errorstr"
	echo
	exit  1
}


if [ -z $RML ]; then
	errorstr="you should first register variable \$RML"
	halt;
fi;


if [ -x /usr/bin/gmake ]; then
	make_tool=gmake
else
    make_tool=make
fi



echo "================ Compiling MorphGen ==================="
$make_tool -C $RML/Source/MorphGen mode=release

if [ "$?" -ne "0" ]; then
	errorstr="cannot compile MorphGen"
	halt;
fi


echo "================ Compiling TestLem ==================="
$make_tool -C $RML/Source/TestLem mode=release

if [ "$?" -ne "0" ]; then
	errorstr="cannot compile TestLem"
	halt;
fi

echo "================ Compiling FileLem ==================="
$make_tool -C $RML/Source/FileLem mode=release

if [ "$?" -ne "0" ]; then
	errorstr="cannot compile FileLem"
	halt;
fi

./gen_graphan_bin.sh Russian
if [ "$?" -ne "0" ]; then
	errorstr="cannot compile graphematical dictionaries for Russian"
	halt;
fi

./gen_graphan_bin.sh German
if [ "$?" -ne "0" ]; then
	errorstr="cannot compile graphematical dictionaries for German"
	halt;
fi


echo
echo "Everything is OK. Try generate_morph_bin.sh"
