#!/bin/sh

rm -Rf doc/html/* && javadoc -d doc/html -sourcepath src_java ru.lspl ru.lspl.patterns ru.lspl.text
cd doc && ./upload.sh
