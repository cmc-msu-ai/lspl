cmake_minimum_required(VERSION 2.6)
project(LSPL_TOOLS)

include(../common/CMakeCommon.cmake)

include_directories( src/main )

### LsplConsole

set(LSPL_CONSOLE_SOURCES
	src/console.cpp
)

add_executable(lspl-console ${LSPL_CONSOLE_SOURCES})

target_link_libraries(lspl-console lspl boost_system)

### LsplGenerator

set(LSPL_GENERATOR_SOURCES
	src/lspl/terms/Term.cpp
	src/lspl/terms/Parser.cpp
	src/generator.cpp
)

add_executable(lspl-gen ${LSPL_GENERATOR_SOURCES})

target_link_libraries(lspl-gen lspl boost_system)

### lspl-find

set(LSPL_FIND_SOURCES
	src/find.cpp
)

add_executable(lspl-find ${LSPL_FIND_SOURCES})

target_link_libraries(lspl-find lspl boost_system)

### lspl-DictionaryRecognizer

set(LSPL_DICTIONARY_RECOGNIZER_SOURCES
	src/DictionaryRecognizer.cpp
	src/DictionaryMain.cpp
	src/RangeSetDecartTreeElement.cpp
	src/RangeSetDecartTree.cpp
	src/RangeSet.cpp
	src/SynDictionary.cpp
	src/Util.cpp
)

add_executable(lspl-dictionary-recognizer ${LSPL_DICTIONARY_RECOGNIZER_SOURCES})

target_link_libraries(lspl-dictionary-recognizer lspl boost_system)

### lspl-Similarity

set(LSPL_SIMILARITY_SOURCES
	src/AbbrAnalyzer.cpp
	src/DictionaryRecognizer.cpp
	src/SimilarityMain.cpp
	src/RangeSetDecartTreeElement.cpp
	src/RangeSetDecartTree.cpp
	src/RangeSet.cpp
	src/SimilarityRecognizer.cpp
	src/SynDictionary.cpp
	src/Util.cpp
	src/WordList.cpp
)

add_executable(lspl-similarity ${LSPL_SIMILARITY_SOURCES})

target_link_libraries(lspl-similarity lspl boost_system)

### lspl-RangeSetDecartTreeTest

set(LSPL_RANGESET_DECART_TREE_TEST_SOURCES
	src/RangeSetDecartTreeTest.cpp
	src/RangeSetDecartTreeElement.cpp
	src/RangeSetDecartTree.cpp
)

add_executable(lspl-rangeset-decart-tree-test ${LSPL_RANGESET_DECART_TREE_TEST_SOURCES})

target_link_libraries(lspl-rangeset-decart-tree-test lspl boost_system)

### lspl-UtilTest

set(LSPL_UTIL_TEST_SOURCES
	src/SynDictionary.cpp
	src/Util.cpp
	src/UtilTest.cpp
)

add_executable(lspl-util-test ${LSPL_UTIL_TEST_SOURCES})

target_link_libraries(lspl-util-test lspl boost_system)

# Flags for effective error parsing

if(CMAKE_COMPILER_IS_GNUCC)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
endif(CMAKE_COMPILER_IS_GNUCC)

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fmessage-length=0")
endif(CMAKE_COMPILER_IS_GNUCXX)


