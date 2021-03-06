#include <jni.h>
#include <stdio.h>

#include <iostream>
#include <sstream>
#include <stdexcept>

#include "lspl_jni/ru_lspl_text_Text.h"

#include "lspl/java/JavaPattern.h"
#include "lspl/java/TextDataBuilderConfig.h"
#include "lspl/java/Utils.h"

#include "lspl/text/java/JavaText.h"
#include "lspl/text/JavaTransition.h"
#include "lspl/text/JavaWord.h"
#include "lspl/text/JavaMatch.h"

#include "lspl/text/Text.h"
#include "lspl/text/Node.h"

using namespace lspl::java;
using namespace lspl::text;
using namespace lspl::text::java;
using namespace lspl::patterns::java;

using lspl::text::markup::WordList;

/*
 * Class:     ru_lspl_text_Text
 * Method:    create
 * Signature: (Ljava/lang/String;)Lru/lspl/text/Text;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_text_Text_create__Ljava_lang_String_2(JNIEnv * env, jclass cls, jstring content) {
	try {
		return JavaText::create( env, content ).object;
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_text_Text
 * Method:    create
 * Signature: (Ljava/lang/String;Lru/lspl/text/TextConfig;)Lru/lspl/text/Text;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_text_Text_create__Ljava_lang_String_2Lru_lspl_text_TextConfig_2(JNIEnv * env, jclass cls, jstring content, jobject config) {
	try {
		return JavaText::create( env, content, TextDataBuilderConfig::fromJava( env, config ) ).object;
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_text_Text
 * Method:    internalGetWords
 * Signature: (I)[Lru/lspl/text/Word;
 */
JNIEXPORT jobjectArray JNICALL Java_ru_lspl_text_Text_internalGetWords(JNIEnv * env, jobject obj_text, jint sp) {
	try {
		const WordList & words = JavaText::get( env, obj_text ).text->getWords( lspl::text::attributes::SpeechPart( sp ) );

		jobjectArray result = env->NewObjectArray( words.size(), JavaWord::clazz, 0 );

		if ( result == 0 )
			return 0;

		for ( int i = 0, sz = words.size(); i < sz; ++ i ) {
			env->SetObjectArrayElement( result, i, JavaTransition::get( env, words[i].get() )->object );
		}

		return result;
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_text_Text
 * Method:    getWord
 * Signature: (II)Lru/lspl/text/Word;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_text_Text_getWord(JNIEnv * env, jobject obj_text, jint sp, jint index) {
	try {
		return JavaTransition::get( env, JavaText::get( env, obj_text ).text->getWords( lspl::text::attributes::SpeechPart( sp ) ).at( index ).get() )->object;
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_text_Text
 * Method:    internalGetMatches
 * Signature: (Lru/lspl/patterns/Pattern;)[Lru/lspl/text/Match;
 */
JNIEXPORT jobjectArray JNICALL Java_ru_lspl_text_Text_internalGetMatches(JNIEnv * env, jobject obj_text, jobject obj_pattern ) {
	try {
		const MatchList & matches = JavaText::get( env, obj_text ).text->getMatches( *JavaPattern::get( env, obj_pattern ).pattern );
		
		jobjectArray result = env->NewObjectArray( matches.size(), JavaMatch::clazz, 0 );
	
		if ( result == 0 )
			return 0;
			
		for ( int i = 0, sz = matches.size(); i < sz; ++ i ) {
			env->SetObjectArrayElement( result, i, JavaTransition::get( env, matches[i].get() )->object );
		}
		
		return result;
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_text_Text
 * Method:    getMatch
 * Signature: (Lru/lspl/Pattern;I)Lru/lspl/text/Match;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_text_Text_getMatch(JNIEnv * env, jobject obj_text, jobject obj_pattern, jint index) {
	try {
		return JavaTransition::get( env, JavaText::get( env, obj_text ).text->getMatches( *JavaPattern::get( env, obj_pattern ).pattern ).at( index ).get() )->object;
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_Text
 * Method:    dump
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_text_Text_dump(JNIEnv * env, jobject obj) {
	try {
		std::ostringstream dump;
		JavaText::get( env, obj ).text->dump( dump );
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
 * Class:     ru_lspl_text_Text
 * Method:    finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_ru_lspl_text_Text_finalize(JNIEnv * env, jobject obj) {
	try {
		JavaText::remove( env, obj );
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
	}
}
