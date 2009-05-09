#include <jni.h>
#include <stdio.h>

#include <sstream>

#include "lspl_jni/ru_lspl_text_attributes_AttributeKey.h"

#include "lspl/text/attributes/AttributeKey.h"
#include "lspl/java/Utils.h"

using namespace lspl::java;
using namespace lspl::text::attributes;

/*
 * Class:     ru_lspl_text_attributes_AttributeKey
 * Method:    getAbbrevation
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_text_attributes_AttributeKey_getAbbrevation(JNIEnv * env, jclass cls, jint id) {
	return out( env, AttributeKey( id ).getAbbrevation() );
}

/*
 * Class:     ru_lspl_text_attributes_AttributeKey
 * Method:    getName
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_text_attributes_AttributeKey_getName(JNIEnv * env, jclass cls, jint id) {
	return out( env, AttributeKey( id ).getName() );
}

/*
 * Class:     ru_lspl_text_attributes_AttributeKey
 * Method:    getTitle
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_text_attributes_AttributeKey_getTitle(JNIEnv * env, jclass cls, jint id) {
	return out( env, AttributeKey( id ).getTitle() );
}
