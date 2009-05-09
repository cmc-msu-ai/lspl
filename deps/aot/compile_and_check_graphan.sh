#!/bin/sh
# Checking Dialing/DWDS tokenizer
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


if [ -x /usr/bin/gmake ]; then
	make_tool=gmake
else
    make_tool=make
fi

echo "================ Compiling GraphmatThick ==================="
$make_tool -C $RML/Source/GraphmatThick mode=release

if [ "$?" -ne "0" ]; then
	errorstr="cannot compile GraphmatThick"
	halt;
fi



echo "======== Checking $1 tokenizer ========"
Bin/GraphmatThick $1 Test/Graphan/$Language/text1.txt -gra Test/Graphan/$Language/text1.gra

cmp Test/Graphan/$Language/text1.gra Test/Graphan/$Language/result.gra

if [ "$?" -ne "0" ]; then
	# rm Test/Graphan/$Language/text1.gra
	errorstr="The tokenizer doesn't work!The simple test failed!"
	halt;
fi

rm Test/Graphan/$Language/text1.gra




echo
echo "Everything is OK."
echo
