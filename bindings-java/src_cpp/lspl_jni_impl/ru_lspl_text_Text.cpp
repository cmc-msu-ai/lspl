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
#include "lspl/text/java/JavaNode.h"
#include "lspl/text/JavaTransition.h"

#include "lspl/text/Text.h"
#include "lspl/text/Node.h"

using namespace lspl::java;
using namespace lspl::text;
using namespace lspl::text::java;
using namespace lspl::patterns::java;

/*
 * Class:     ru_lspl_text_Text
 * Method:    create
 * Signature: (Ljava/lang/String;)Lru/lspl/text/Text;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_text_Text_create__Ljava_lang_String_2(JNIEnv * env, jclass cls, jstring content) {
	return JavaText::create( env, content ).object;
}

/*
 * Class:     ru_lspl_text_Text
 * Method:    create
 * Signature: (Ljava/lang/String;Lru/lspl/text/TextConfig;)Lru/lspl/text/Text;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_text_Text_create__Ljava_lang_String_2Lru_lspl_text_TextConfig_2(JNIEnv * env, jclass cls, jstring content, jobject config) {
	return JavaText::create( env, content, TextDataBuilderConfig::fromJava( env, config ) ).object;
}

/*
 * Class:     ru_lspl_Text
 * Method:    getNodeCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_ru_lspl_text_Text_getNodeCount(JNIEnv * env, jobject obj_text) {
	return JavaText::get( env, obj_text ).text->getNodes().size();
}

/*
 * Class:     ru_lspl_Text
 * Method:    getNode
 * Signature: (I)Lru/lspl/text/Node;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_text_Text_getNode(JNIEnv * env, jobject obj_text, jint index) {
	return JavaNode::get( env, JavaText::get( env, obj_text ).text->getNodes().at( index ).get() );
}

/*
 * Class:     ru_lspl_text_Text
 * Method:    getWordCount
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_ru_lspl_text_Text_getWordCount(JNIEnv * env, jobject obj_text, jint sp) {
	return JavaText::get( env, obj_text ).text->getWords( lspl::text::attributes::SpeechPart( sp ) ).size();
}

/*
 * Class:     ru_lspl_text_Text
 * Method:    getWord
 * Signature: (II)Lru/lspl/text/Word;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_text_Text_getWord(JNIEnv * env, jobject obj_text, jint sp, jint index) {
	return JavaTransition::get( env, JavaText::get( env, obj_text ).text->getWords( lspl::text::attributes::SpeechPart( sp ) ).at( index ).get() )->object;
}

/*
 * Class:     ru_lspl_text_Text
 * Method:    getMatchCount
 * Signature: (Lru/lspl/Pattern;)I
 */
JNIEXPORT jint JNICALL Java_ru_lspl_text_Text_getMatchCount(JNIEnv * env, jobject obj_text, jobject obj_pattern ) {
	try {
		return JavaText::get( env, obj_text ).text->getMatches( *JavaPattern::get( env, obj_pattern ).pattern ).size();
	} catch ( const std::exception & e ) {
		std::cerr << "!!!" << e.what() << std::endl;
	} catch ( ... ) {
		std::cerr << "!!!???" << std::endl;
	}

	return 0;
}

/*
 * Class:     ru_lspl_text_Text
 * Method:    getMatch
 * Signature: (Lru/lspl/Pattern;I)Lru/lspl/text/Match;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_text_Text_getMatch(JNIEnv * env, jobject obj_text, jobject obj_pattern, jint index) {
	try {
		return JavaTransition::get( env, JavaText::get( env, obj_text ).text->getMatches( *JavaPattern::get( env, obj_pattern ).pattern ).at( index ).get() )->object;
	} catch ( const std::exception & e ) {
		std::cerr << "!!!" << e.what() << std::endl;
	} catch ( ... ) {
		std::cerr << "!!!???" << std::endl;
	}

	return 0;
}

/*
 * Class:     ru_lspl_Text
 * Method:    dump
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_text_Text_dump(JNIEnv * env, jobject obj) {
	std::ostringstream dump;
	JavaText::get( env, obj ).text->dump( dump );
	return out( env, dump.str() );
}

/*
 * Class:     ru_lspl_text_Text
 * Method:    finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_ru_lspl_text_Text_finalize(JNIEnv * env, jobject obj) {
	JavaText::remove( env, obj );
}
