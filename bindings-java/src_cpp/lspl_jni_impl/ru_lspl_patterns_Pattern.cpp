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
	return JavaPattern::get( env, obj )->alternatives.size();
}

/*
 * Class:     ru_lspl_Pattern
 * Method:    getAlternative
 * Signature: (I)Lru/lspl/Alternative;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_patterns_Pattern_getAlternative(JNIEnv * env, jobject obj, jint index) {
	JavaPattern pattern = JavaPattern::get( env, obj );

	return JavaAlternative::get( env, pattern.pattern.get(), &pattern->alternatives[ index ] ).object;
}

/*
 * Class:     ru_lspl_patterns_Pattern
 * Method:    getDependencyCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_ru_lspl_patterns_Pattern_getDependencyCount(JNIEnv * env, jobject obj) {
	return JavaPattern::get( env, obj )->getDependencies().size();
}

/*
 * Class:     ru_lspl_patterns_Pattern
 * Method:    getDependency
 * Signature: (I)Lru/lspl/patterns/Pattern;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_patterns_Pattern_getDependency(JNIEnv * env, jobject obj, jint index) {
	return JavaPattern::get( env, JavaPattern::get( env, obj )->getDependencies().at( index ) ).object;
}

/*
 * Class:     ru_lspl_Pattern
 * Method:    getSource
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_patterns_Pattern_getSource(JNIEnv * env, jobject obj) {
	return out( env, JavaPattern::get( env, obj )->getSource() );
}

/*
 * Class:     ru_lspl_Pattern
 * Method:    dump
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_patterns_Pattern_dump(JNIEnv * env, jobject obj) {
	std::ostringstream dump;
	JavaPattern::get( env, obj )->dump( dump );
	return out( env, dump.str() );
}

/*
 * Class:     ru_lspl_patterns_Pattern
 * Method:    removeDuplicateAlternatives
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_ru_lspl_patterns_Pattern_removeDuplicateAlternatives(JNIEnv * env, jobject obj) {
	JavaPattern::get( env, obj )->removeDuplicateAlternatives();
}

/*
 * Class:     ru_lspl_Pattern
 * Method:    finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_ru_lspl_patterns_Pattern_finalize(JNIEnv * env, jobject obj) {
	JavaPattern::remove( env, obj );
}
