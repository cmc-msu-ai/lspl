###############################################################################
# Makefile for LSPL project
##############################################################################

all: gui
all-win32: gui-win32

###############################################################################
# AOT
###############################################################################

./deps/aot/Source/LemmatizerLib/libLemmatizerrst.a:
	cd deps/aot && ./build-linux.sh

./deps/aot/Source/LemmatizerLib/libLemmatizermrst.a:
	cd deps/aot && ./build-win32.sh

aot: ./deps/aot/Source/LemmatizerLib/libLemmatizerrst.a
aot-win32: ./deps/aot/Source/LemmatizerLib/libLemmatizermrst.a

# Core

core: aot
	make -C core linux64
core-win32: aot-win32
	make -C core win32

# Java bindings

java: core
	make -C bindings-java linux64
java-win32: core-win32
	make -C bindings-java win32

###############################################################################
# Tools
###############################################################################

./tools/build/Makefile:
	cd tools && ./configure-linux.sh

./tools/build-win32/Makefile:
	cd tools && ./configure-win32.sh

./tools/build/lspl-find: ./core/build/liblspl.so ./tools/build/Makefile
	make -C tools/build

./tools/build-win32/lspl-find.exe: ./core/build-win32/lspl.dll ./tools/build-win32/Makefile
	make -C tools/build-win32

tools: ./tools/build/lspl-find
tools-win32: ./tools/build-win32/lspl-find.exe

###############################################################################
# GUI
###############################################################################

gui: core java tools
	cd gui && ant package

gui-win32: core-win32 java-win32 tools-win32
	cd gui && ant package