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
	try {
		return out( env, AttributeValue( id ).getAbbrevation() );
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_text_attributes_IndexedAttribute
 * Method:    getName
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_text_attributes_IndexedAttribute_getName(JNIEnv * env, jclass cls, jint id) {
	try {
		return out( env, AttributeValue( id ).getName() );
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_text_attributes_IndexedAttribute
 * Method:    getTitle
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_text_attributes_IndexedAttribute_getTitle(JNIEnv * env, jclass cls, jint id) {
	try {
		return out( env, AttributeValue( id ).getTitle() );
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}
