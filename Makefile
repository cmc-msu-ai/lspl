###############################################################################
# Makefile for LSPL project
##############################################################################

all: core

###############################################################################
# AOT
###############################################################################

./deps/aot/Source/LemmatizerLib/libLemmatizerrst.a:
	cd deps/aot && ./build-linux.sh

aot: ./deps/aot/Source/LemmatizerLib/libLemmatizerrst.a

# Core

core: aot
	make -C core linux64
core-mac: aot
	make -C core mac
core-mac-xcode: aot
	make -C core mac-xcode

install:
	cp core/build-linux64/liblspl.so /usr/lib/
	cp -r core/src/main/lspl /usr/include
	cp -r deps/aot /usr/share

