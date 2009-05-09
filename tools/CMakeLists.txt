cmake_minimum_required(VERSION 2.6)
project(LSPL_TOOLS)

include_directories( ../deps/ ../core/src/main/ )
link_directories(
	${LSPL_TOOLS_SOURCE_DIR}/../core/build/
	${LSPL_TOOLS_SOURCE_DIR}/../core/build-win32/
)

### LsplConsole

set(LSPL_CONSOLE_SOURCES
	src/console.cpp
)

add_executable(lspl-console ${LSPL_CONSOLE_SOURCES})

target_link_libraries(lspl-console lspl)

### LsplGenerator

set(LSPL_GENERATOR_SOURCES
    src/lspl/terms/Term.cpp
    src/lspl/terms/Parser.cpp
	src/generator.cpp
)

add_executable(lspl-gen ${LSPL_GENERATOR_SOURCES})

target_link_libraries(lspl-gen lspl)

### lspl-find

set(LSPL_FIND_SOURCES
	src/find.cpp
)

add_executable(lspl-find ${LSPL_FIND_SOURCES})

target_link_libraries(lspl-find lspl)

# Flags for effective error parsing

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
endif(CMAKE_COMPILER_IS_GNUCC)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
endif(CMAKE_COMPILER_IS_GNUCXX)


