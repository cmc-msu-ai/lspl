#!/bin/sh
# Checking Dialing/DWDS morphology
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


for i in Russian English German; do
	if [ $i = $1 ]; then
		#Language=`expr substr $i 1 3`
		Language=`echo $i | awk "{print substr(\\$0,1,3);}"`
		break;
	fi
done

if [ -z $Language ]; then
	bad_arguments;
fi


if [ -z $RML ]; then
	errorstr="you should first register variable \$RML"
	halt;
fi;

if [ ! -x $RML/Source/MorphGen/MorphGen ]; then
	errorstr="you should first call compile_morph.sh"
	halt;
fi;

if [ ! -e $RML/Dicts/SrcMorph/$Language.mwz ] ; then
	echo "Error: you should first download and install the morphological dictionary (http://sf.net/projects/morph-lexicon/)"
	exit 1
fi


echo "==== Generating binary morphological files  ==============="

$RML/Source/MorphGen/MorphGen Dicts/SrcMorph/$Language.mwz Dicts/Morph/$Language 5 3

if [ "$?" -ne "0" ]; then
	errorstr="cannot generate morphological dictionaries"
	halt;
fi



echo "======== Checking $1 Dictionary with the simple test ========"
Bin/TestLem $1 <Test/Morph/$Language/test.txt  >test.temp

cmp test.temp Test/Morph/$Language/result.txt

if [ "$?" -ne "0" ]; then
	rm test.temp
	errorstr="The dictionary doesn't work!The simple test failed!"
	halt;
fi

rm test.temp



echo
echo "Everything is OK. Try 'Bin/TestLem $1' in order to test morphology manually"
