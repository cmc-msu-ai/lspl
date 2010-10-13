#include <jni.h>
#include <stdio.h>

#include <sstream>

#include "lspl_jni/ru_lspl_patterns_Alternative.h"

#include "lspl/java/JavaAlternative.h"
#include "lspl/java/JavaPattern.h"
#include "lspl/java/Utils.h"

#include "lspl/patterns/Pattern.h"
#include "lspl/patterns/Alternative.h"

using namespace lspl::java;
using namespace lspl::patterns;
using namespace lspl::patterns::java;

/*
 * Class:     ru_lspl_patterns_Alternative
 * Method:    getDependencyCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_ru_lspl_patterns_Alternative_getDependencyCount(JNIEnv * env, jobject obj) {
	try {
		return JavaAlternative::get( env, obj )->getDependencies().size();
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_patterns_Alternative
 * Method:    getDependency
 * Signature: (I)Lru/lspl/patterns/Pattern;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_patterns_Alternative_getDependency(JNIEnv * env, jobject obj, jint index) {
	try {
		return JavaPattern::get( env, JavaAlternative::get( env, obj )->getDependencies().at( index ) ).object;
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_Alternative
 * Method:    getMatcherCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_ru_lspl_patterns_Alternative_getMatcherCount(JNIEnv * env, jobject obj) {
	try {
		return JavaAlternative::get( env, obj )->getMatcherCount();
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_Alternative
 * Method:    getSource
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_patterns_Alternative_getSource(JNIEnv * env, jobject obj) {
	try {
		return out( env, JavaAlternative::get( env, obj )->getSource() );
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_Alternative
 * Method:    dump
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_patterns_Alternative_dump(JNIEnv * env, jobject obj) {
	try {
		std::ostringstream dump;
		JavaAlternative::get( env, obj )->dump( dump );
		return out( env, dump.str() );
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_Alternative
 * Method:    finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_ru_lspl_patterns_Alternative_finalize(JNIEnv * env, jobject obj) {
	try {
		JavaAlternative::remove( env, obj );
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
	}
}
