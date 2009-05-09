#include <jni.h>
#include <stdio.h>

#include <sstream>

#include "lspl_jni/ru_lspl_text_attributes_IndexedAttribute.h"

#include "lspl/text/attributes/AttributeValue.h"
#include "lspl/java/Utils.h"

using namespace lspl::java;
using namespace lspl::text::attributes;

/*
 * Class:     ru_lspl_text_attributes_IndexedAttribute
 * Method:    getAbbrevation
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_text_attributes_IndexedAttribute_getAbbrevation(JNIEnv * env, jclass cls, jint id) {
	return out( env, AttributeValue( id ).getAbbrevation() );
}

/*
 * Class:     ru_lspl_text_attributes_IndexedAttribute
 * Method:    getName
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_text_attributes_IndexedAttribute_getName(JNIEnv * env, jclass cls, jint id) {
	return out( env, AttributeValue( id ).getName() );
}

/*
 * Class:     ru_lspl_text_attributes_IndexedAttribute
 * Method:    getTitle
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_text_attributes_IndexedAttribute_getTitle(JNIEnv * env, jclass cls, jint id) {
	return out( env, AttributeValue( id ).getTitle() );
}
