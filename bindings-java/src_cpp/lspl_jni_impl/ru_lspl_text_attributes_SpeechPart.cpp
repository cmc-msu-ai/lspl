#include <jni.h>
#include <stdio.h>

#include <sstream>

#include "lspl_jni/ru_lspl_text_attributes_SpeechPart.h"

#include "lspl/text/attributes/SpeechPart.h"
#include "lspl/java/Utils.h"

using namespace lspl::java;
using namespace lspl::text::attributes;

/*
 * Class:     ru_lspl_text_attributes_SpeechPart
 * Method:    getAbbrevation
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_text_attributes_SpeechPart_getAbbrevation(JNIEnv * env, jclass cls, jint id) {
	return out( env, SpeechPart( id ).getAbbrevation() );
}

/*
 * Class:     ru_lspl_text_attributes_SpeechPart
 * Method:    getName
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_text_attributes_SpeechPart_getName(JNIEnv * env, jclass cls, jint id) {
	return out( env, SpeechPart( id ).getName() );
}

/*
 * Class:     ru_lspl_text_attributes_SpeechPart
 * Method:    getTitle
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_text_attributes_SpeechPart_getTitle(JNIEnv * env, jclass cls, jint id) {
	return out( env, SpeechPart( id ).getTitle() );
}

/*
 * Class:     ru_lspl_text_attributes_SpeechPart
 * Method:    getPluralTitle
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_text_attributes_SpeechPart_getPluralTitle(JNIEnv * env, jclass cls, jint id) {
	return out( env, SpeechPart( id ).getPluralTitle() );
}
