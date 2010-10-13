#include <jni.h>
#include <stdio.h>

#include <sstream>

#include "lspl_jni/ru_lspl_patterns_Pattern.h"

#include "lspl/java/JavaAlternative.h"
#include "lspl/java/JavaPattern.h"
#include "lspl/java/Utils.h"

#include "lspl/patterns/Pattern.h"

using namespace lspl::java;
using namespace lspl::patterns;
using namespace lspl::patterns::java;

/*
 * Class:     ru_lspl_Pattern
 * Method:    getAlternativeCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_ru_lspl_patterns_Pattern_getAlternativeCount(JNIEnv * env, jobject obj) {
	try {
		return JavaPattern::get( env, obj )->getAlternatives().size();
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_Pattern
 * Method:    getAlternative
 * Signature: (I)Lru/lspl/Alternative;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_patterns_Pattern_getAlternative(JNIEnv * env, jobject obj, jint index) {
	try {
		JavaPattern pattern = JavaPattern::get( env, obj );

		return JavaAlternative::get( env, pattern.pattern.get(), &pattern->getAlternatives().at( index ) ).object;
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_patterns_Pattern
 * Method:    getDependencyCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_ru_lspl_patterns_Pattern_getDependencyCount(JNIEnv * env, jobject obj) {
	try {
		return JavaPattern::get( env, obj )->getDependencies().size();
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_patterns_Pattern
 * Method:    getDependency
 * Signature: (I)Lru/lspl/patterns/Pattern;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_patterns_Pattern_getDependency(JNIEnv * env, jobject obj, jint index) {
	try {
		return JavaPattern::get( env, JavaPattern::get( env, obj )->getDependencies().at( index ) ).object;
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_Pattern
 * Method:    getSource
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_patterns_Pattern_getSource(JNIEnv * env, jobject obj) {
	try {
		return out( env, JavaPattern::get( env, obj )->getSource() );
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_Pattern
 * Method:    dump
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_patterns_Pattern_dump(JNIEnv * env, jobject obj) {
	try {
		std::ostringstream dump;
		JavaPattern::get( env, obj )->dump( dump );
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
 * Class:     ru_lspl_patterns_Pattern
 * Method:    removeDuplicateAlternatives
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_ru_lspl_patterns_Pattern_removeDuplicateAlternatives(JNIEnv * env, jobject obj) {
	try {
		JavaPattern::get( env, obj )->removeDuplicateAlternatives();
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
	}
}

/*
 * Class:     ru_lspl_Pattern
 * Method:    finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_ru_lspl_patterns_Pattern_finalize(JNIEnv * env, jobject obj) {
	try {
		JavaPattern::remove( env, obj );
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
	}
}
