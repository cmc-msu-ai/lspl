cmake_minimum_required(VERSION 2.6)
project(LSPL_JAVA)

find_package(Boost)
find_package(JNI)

if(MINGW)
    SET(MY_TARGETLIB_PREFIX "")
    SET(MY_TARGETLIB_SUFFIX ".dll")
else(MINGW)
    SET(MY_TARGETLIB_PREFIX "lib")
    SET(MY_TARGETLIB_SUFFIX ".so")
endif(MINGW)

include_directories( ../deps/ ../core/src/main/ src_cpp ${JNI_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} )
link_directories(
	${LSPL_JAVA_SOURCE_DIR}/../core/build/
	${LSPL_JAVA_SOURCE_DIR}/../core/build-win32/
)

### lspl.jar

ADD_CUSTOM_TARGET(lspl.jar
  COMMAND ant -q -f "${LSPL_JAVA_SOURCE_DIR}/build.xml"

  COMMENT "Compiling java code"
  VERBATIM
)

### jni-stubs

ADD_CUSTOM_TARGET(jni-stubs
  COMMAND javah
         -jni
         -classpath "${LSPL_JAVA_SOURCE_DIR}/build/lspl.jar"
         -d "${LSPL_JAVA_SOURCE_DIR}/src_cpp/lspl_jni"
         ru.lspl.patterns.Pattern
         ru.lspl.patterns.PatternBuilder
         ru.lspl.text.Text
         ru.lspl.patterns.Alternative
         ru.lspl.text.Node
         ru.lspl.text.Transition
         ru.lspl.text.Loop
         ru.lspl.text.Match
         ru.lspl.LsplObject
         ru.lspl.text.attributes.SpeechPart
         ru.lspl.text.attributes.IndexedAttribute
         ru.lspl.text.attributes.AttributeKey

  COMMENT "Generating JNI stubs"
  VERBATIM
)

add_dependencies(jni-stubs lspl.jar)

### lspl-java library

set(LSPL_JAVA_SOURCES
	src_cpp/lspl/java/JavaMatcher.cpp
	src_cpp/lspl/java/JavaPatternBuilder.cpp
	src_cpp/lspl/java/JavaPattern.cpp
	src_cpp/lspl/java/TextDataBuilderConfig.cpp
	src_cpp/lspl/java/JavaAlternative.cpp
	src_cpp/lspl/java/Utils.cpp
	src_cpp/lspl/text/java/JavaText.cpp
	src_cpp/lspl/text/java/JavaNode.cpp
	src_cpp/lspl/text/JavaTransition.cpp
	src_cpp/lspl/text/JavaLoop.cpp
	src_cpp/lspl/text/JavaMatch.cpp
	src_cpp/lspl/text/JavaToken.cpp
	src_cpp/lspl/text/JavaWord.cpp
	src_cpp/lspl/text/attributes/java/JavaAttributeValue.cpp
	src_cpp/lspl/transforms/JavaTransform.cpp
	src_cpp/lspl/transforms/JavaTransformBuilder.cpp

	src_cpp/lspl_jni_impl/ru_lspl_LsplObject.cpp
	
	src_cpp/lspl_jni_impl/ru_lspl_patterns_Alternative.cpp
	src_cpp/lspl_jni_impl/ru_lspl_patterns_Pattern.cpp
	src_cpp/lspl_jni_impl/ru_lspl_patterns_PatternBuilder.cpp
	
	src_cpp/lspl_jni_impl/ru_lspl_text_attributes_AttributeKey.cpp
	src_cpp/lspl_jni_impl/ru_lspl_text_attributes_IndexedAttribute.cpp	
	src_cpp/lspl_jni_impl/ru_lspl_text_attributes_SpeechPart.cpp
	
	src_cpp/lspl_jni_impl/ru_lspl_text_Loop.cpp
	src_cpp/lspl_jni_impl/ru_lspl_text_Match.cpp
	src_cpp/lspl_jni_impl/ru_lspl_text_Node.cpp
	src_cpp/lspl_jni_impl/ru_lspl_text_Text.cpp
	src_cpp/lspl_jni_impl/ru_lspl_text_Transition.cpp
)

add_library(lspl-java SHARED ${LSPL_JAVA_SOURCES})
add_dependencies(lspl-java lspl.jar jni-stubs)

target_link_libraries(lspl-java lspl)

set_target_properties(lspl-java
    PROPERTIES
    PREFIX "${MY_TARGETLIB_PREFIX}"
    SUFFIX "${MY_TARGETLIB_SUFFIX}"
)

# Flags for effective error parsing

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
endif(CMAKE_COMPILER_IS_GNUCC)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
endif(CMAKE_COMPILER_IS_GNUCXX)

