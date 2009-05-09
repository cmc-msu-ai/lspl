#!/bin/sh
# Generating Struct-dictionaries for Graphematics
# written by Alexey Sokirko 2003, sokirko@yandex.ru

halt() {
	echo  "======================"
	echo  "Error: $errorstr"
	echo
	exit  1
}

bad_arguments() {
	errorstr="the first argument should be Russian, English or German"
	halt;
}



if [ $# != 1 ]; then
	bad_arguments;
fi



if [ $1 = "Russian" ]; then
	./compile_ross.sh Dicts/Obor
	if [ "$?" -ne "0" ]; then
		halt;
	fi
elif [ $1 = "German" ]; then
	./compile_ross.sh Dicts/GerObor
	if [ "$?" -ne "0" ]; then
		halt;
	fi

else
  echo " unsupported language"
  halt;   
fi



echo
echo  "Everything is OK!"


