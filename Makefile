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

###############################################################################
# Core
###############################################################################

./core/build/Makefile:
	cd core && ./configure-linux.sh

./core/build-win32/Makefile:
	cd core && ./configure-win32.sh

./core/build/liblspl.so: aot ./core/build/Makefile
	make -C core/build

./core/build-win32/lspl.dll: aot-win32 ./core/build-win32/Makefile
	make -C core/build-win32

core: ./core/build/liblspl.so
core-win32: ./core/build-win32/lspl.dll

###############################################################################
# Java
###############################################################################

./bindings-java/build/Makefile:
	cd bindings-java && ./configure-linux.sh

./bindings-java/build-win32/Makefile:
	cd bindings-java && ./configure-win32.sh

./bindings-java/build/liblspl-java.so: ./core/build/liblspl.so ./bindings-java/build/Makefile
	make -C bindings-java/build

./bindings-java/build-win32/lspl-java.dll: ./core/build-win32/lspl.dll ./bindings-java/build-win32/Makefile
	make -C bindings-java/build-win32

java: ./bindings-java/build/liblspl-java.so
java-win32: ./bindings-java/build-win32/lspl-java.dll

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