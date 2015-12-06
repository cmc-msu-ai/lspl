# Linux64

linux64: ./build-linux64/Makefile
	make -C build-linux64

clean-linux64: ./build-linux64/Makefile
	make -C build-linux64 clean

distclean-linux64:
	rm -r build-linux64

# Mac

mac: ./build-mac/Makefile
	make -j8 -C build-mac

mac-xcode: ./build-mac-xcode/Makefile

clean-mac: ./build-mac/Makefile
	make -C build-mac clean

distclean-mac:
	rm -r build-mac

# Win32

win32: ./build-win32/Makefile
	make -C build-win32

clean-win32: ./build-win32/Makefile
	make -C build-win32 clean

distclean-win32:
	rm -r build-win32

# Configuration rules

./build-linux64/Makefile:
	mkdir -p build-linux64 && cd build-linux64 && cmake -DCMAKE_SYSTEM_NAME=Linux .. -DCMAKE_BUILD_TYPE=Release ..

./build-mac/Makefile:
	mkdir -p build-mac && cd build-mac && cmake -DCMAKE_SYSTEM_NAME=Darwin .. -DCMAKE_BUILD_TYPE=Release ..

./build-mac-xcode/Makefile:
	mkdir -p build-mac-xcode && cd build-mac-xcode && cmake -DCMAKE_SYSTEM_NAME=Darwin .. -DCMAKE_BUILD_TYPE=Release .. -G Xcode

./build-win32/Makefile:
	mkdir -p build-win32 && cd build-win32 && cmake -DCMAKE_TOOLCHAIN_FILE=../common/Toolchain-mingw32.cmake -DCMAKE_BUILD_TYPE=Release ..
