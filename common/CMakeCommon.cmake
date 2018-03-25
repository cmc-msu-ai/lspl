find_package(Boost)

if(WIN32)
    SET(ICONV_PATH ${CMAKE_SOURCE_DIR}/../deps-win32/libiconv-1.9.2-1/)
    SET(PCRE_PATH ${CMAKE_SOURCE_DIR}/../deps-win32/PCRE-8.38/)

    include_directories(
        ${CMAKE_SOURCE_DIR}/../core/src/main/
        ${CMAKE_SOURCE_DIR}/../deps/
        ${ICONV_PATH}/include/
        ${PCRE_PATH}/include/
        ${Boost_INCLUDE_DIR}
    )

    link_directories(
        ${CMAKE_SOURCE_DIR}/../deps/aot/Source/
        ${ICONV_PATH}/lib/
        ${PCRE_PATH}/lib/
        ${CMAKE_BINARY_DIR}/
    )

    SET(MY_TARGETLIB_PREFIX "")
    SET(MY_TARGETLIB_SUFFIX ".dll")
elseif(APPLE)
    include_directories( ../core/src/main/ ../deps/ ${Boost_INCLUDE_DIRS} )

    link_directories(
        ${CMAKE_SOURCE_DIR}/../deps/aot/Source/AgramtabLib/
        ${CMAKE_SOURCE_DIR}/../deps/aot/Source/GraphanLib/
        ${CMAKE_SOURCE_DIR}/../deps/aot/Source/LemmatizerLib/
        ${CMAKE_SOURCE_DIR}/../deps/aot/Source/MorphWizardLib/
        ${CMAKE_SOURCE_DIR}/../deps/aot/Source/StructDictLib/
        ${CMAKE_SOURCE_DIR}/../core/build-mac/
        ${Boost_LIBRARY_DIRS}
    )

    SET(MY_TARGETLIB_PREFIX "lib")
    SET(MY_TARGETLIB_SUFFIX ".so")
else(WIN32)
    include_directories( ../core/src/main/ ../deps/ ${Boost_INCLUDE_DIR} )

    link_directories(
        ${CMAKE_SOURCE_DIR}/../deps/aot/Source/AgramtabLib/
        ${CMAKE_SOURCE_DIR}/../deps/aot/Source/GraphanLib/
        ${CMAKE_SOURCE_DIR}/../deps/aot/Source/LemmatizerLib/
        ${CMAKE_SOURCE_DIR}/../deps/aot/Source/MorphWizardLib/
        ${CMAKE_SOURCE_DIR}/../deps/aot/Source/StructDictLib/
        ${CMAKE_SOURCE_DIR}/../core/build-linux64/
    )

    SET(MY_TARGETLIB_PREFIX "lib")
    SET(MY_TARGETLIB_SUFFIX ".so")
endif(WIN32)