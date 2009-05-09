#!/bin/sh
# Compiling Dialing StrucuDict Loaded
# the script is written by Alexey Sokirko 2003, sokirko@yandex.ru

halt() {
	echo  "======================"
	echo  "Error: $errorstr"
	echo
	exit  1
}

bad_arguments() {
	errorstr="the first argument should be a name of Structural Dictionary"
	halt;
}



if [ $# != 1 ]; then
	bad_arguments;
fi

if [ -x /usr/bin/gmake ]; then
	make_tool=gmake
else
    make_tool=make
fi


echo "================ Compiling StructDictLoader ==================="
$make_tool -C $RML/Source/StructDictLoader mode=release

if [ "$?" -ne "0" ]; then
	errorstr="cannot compile StructDictLoader"
	halt;
fi

echo "======== Compiling $1  ========"
Bin/StructDictLoader FromTxt $1/ross.txt  $1
if [ "$?" -ne "0" ]; then
	errorstr="Cannot compile $1!"
	halt;
fi

