find_package(Boost)

if(WIN32)
    include_directories( ../core/src/main/ ../deps/ ../deps-win32/include/ ${Boost_INCLUDE_DIR} )

    link_directories(
        ${CMAKE_SOURCE_DIR}/../deps/aot/Source/AgramtabLib/
        ${CMAKE_SOURCE_DIR}/../deps/aot/Source/GraphanLib/
        ${CMAKE_SOURCE_DIR}/../deps/aot/Source/LemmatizerLib/
        ${CMAKE_SOURCE_DIR}/../deps/aot/Source/MorphWizardLib/
        ${CMAKE_SOURCE_DIR}/../deps/aot/Source/StructDictLib/
        ${CMAKE_SOURCE_DIR}/../deps-win32/lib/
        ${CMAKE_SOURCE_DIR}/../core/build-win32/
    )

    SET(MY_TARGETLIB_PREFIX "")
    SET(MY_TARGETLIB_SUFFIX ".dll")
else(WIN32)
    include_directories( ../core/src/main/ ../deps/ ${Boost_INCLUDE_DIR} )

    link_directories(
        ${CMAKE_SOURCE_DIR}/../deps/aot/Source/AgramtabLib/
        ${CMAKE_SOURCE_DIR}/../deps/aot/Source/GraphanLib/
        ${CMAKE_SOURCE_DIR}/../deps/aot/Source/LemmatizerLib/
        ${CMAKE_SOURCE_DIR}/../deps/aot/Source/MorphWizardLib/
        ${CMAKE_SOURCE_DIR}/../deps/aot/Source/StructDictLib/
        ${CMAKE_SOURCE_DIR}/../core/build/
    )

    SET(MY_TARGETLIB_PREFIX "lib")
    SET(MY_TARGETLIB_SUFFIX ".so")
endif(WIN32)