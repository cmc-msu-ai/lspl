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
	try {
		return out( env, SpeechPart( id ).getAbbrevation() );
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_text_attributes_SpeechPart
 * Method:    getName
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_text_attributes_SpeechPart_getName(JNIEnv * env, jclass cls, jint id) {
	try {
		return out( env, SpeechPart( id ).getName() );
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_text_attributes_SpeechPart
 * Method:    getTitle
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_text_attributes_SpeechPart_getTitle(JNIEnv * env, jclass cls, jint id) {
	try {
		return out( env, SpeechPart( id ).getTitle() );
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_text_attributes_SpeechPart
 * Method:    getPluralTitle
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_text_attributes_SpeechPart_getPluralTitle(JNIEnv * env, jclass cls, jint id) {
	try {
		return out( env, SpeechPart( id ).getPluralTitle() );
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}
