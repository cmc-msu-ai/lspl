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

tools: core
	make -C tools linux64
tools-win32: core-win32
	make -C tools win32

###############################################################################
# GUI
###############################################################################

gui: core java tools
	cd gui && ant package

gui-win32: core-win32 java-win32 tools-win32
	cd gui && ant package