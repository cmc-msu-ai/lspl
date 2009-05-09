#include <jni.h>
#include <stdio.h>

#include <sstream>
#include <map>

#include "lspl_jni/ru_lspl_patterns_PatternBuilder.h"

#include "lspl/java/JavaPatternBuilder.h"
#include "lspl/java/JavaPattern.h"
#include "lspl/java/Utils.h"

#include "lspl/patterns/PatternBuilder.h"
#include "lspl/patterns/Pattern.h"

using namespace lspl::java;
using namespace lspl::patterns;
using namespace lspl::patterns::java;

/*
 * Class:     ru_lspl_PatternBuilder
 * Method:    create
 * Signature: ()I
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_patterns_PatternBuilder_create(JNIEnv * env, jclass) {
	return JavaPatternBuilder::get( env, new PatternBuilder() ).object;
}

/*
 * Class:     ru_lspl_PatternBuilder
 * Method:    getDefinedPatternCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_ru_lspl_patterns_PatternBuilder_getDefinedPatternCount(JNIEnv * env, jobject obj) {
	return JavaPatternBuilder::get( env, obj ).builder->space->getPatternCount();
}

/*
 * Class:     ru_lspl_PatternBuilder
 * Method:    getDefinedPattern
 * Signature: (I)Lru/lspl/Pattern;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_patterns_PatternBuilder_getDefinedPattern__I(JNIEnv * env, jobject obj, jint index) {
	JavaPatternBuilder & data = JavaPatternBuilder::get( env, obj );

	return JavaPattern::get( env, data.builder->space->getPatternByIndex( index ).get() ).object;
}

/*
 * Class:     ru_lspl_PatternBuilder
 * Method:    getDefinedPattern
 * Signature: (Ljava/lang/String;)Lru/lspl/Pattern;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_patterns_PatternBuilder_getDefinedPattern__Ljava_lang_String_2(JNIEnv * env, jobject obj, jstring name) {
	JavaPatternBuilder & data = JavaPatternBuilder::get( env, obj );

	Pattern * pattern = data.builder->space->getPatternByName( in( env, name ) ).get();

	if ( !pattern )
		return 0;

	return JavaPattern::get( env, pattern ).object;
}

/*
 * Class:     ru_lspl_PatternBuilder
 * Method:    build
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_ru_lspl_patterns_PatternBuilder_build(JNIEnv * env, jobject obj, jstring source) {
	try {
		JavaPatternBuilder::get( env, obj ).builder->build( in( env, source ) );
	} catch ( std::exception & ex ) {
		env->ThrowNew( env->FindClass("ru/lspl/patterns/PatternBuildingException"), ex.what() );
	}
}

/*
 * Class:     ru_lspl_PatternBuilder
 * Method:    finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_ru_lspl_patterns_PatternBuilder_finalize(JNIEnv * env, jobject obj) {
	JavaPatternBuilder::remove( env, obj );
}
