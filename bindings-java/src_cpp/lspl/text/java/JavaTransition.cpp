#include "JavaTransition.h"
#include "JavaNode.h"
#include "JavaText.h"

#include "lspl/java/Utils.h"
#include "lspl/java/JavaPattern.h"

#include "lspl/patterns/Pattern.h"

#include "lspl/base/Exception.h"
#include "lspl/text/Transition.h"
#include "lspl/text/markup/Word.h"
#include "lspl/text/Match.h"
#include "lspl/text/markup/Token.h"
#include "lspl/text/Node.h"
#include "lspl/text/Text.h"

#include <iostream>

using lspl::java::out;

using lspl::patterns::Pattern;

using lspl::text::Text;
using lspl::text::Transition;
using lspl::text::markup::Token;
using lspl::text::markup::Word;
using lspl::text::Match;

#include <iostream>

namespace lspl { namespace text { namespace java {

jclass JavaTransition::transitionClazz;
jmethodID JavaTransition::transitionConstructor;
jfieldID JavaTransition::transitionIdField;

jclass JavaTransition::wordClazz;
jmethodID JavaTransition::wordConstructor;

jclass JavaTransition::matchClazz;
jmethodID JavaTransition::matchConstructor;

jclass JavaTransition::punctuationClazz;
jmethodID JavaTransition::punctuationConstructor;

std::vector<JavaTransition *> JavaTransition::transitions;

JavaTransition::JavaTransition( TransitionRef transition, JNIEnv * env ) :
	transition( transition ) {

	jobject textObj = JavaText::get( env, const_cast<Text*>( &transition->start.text ) ).object;
	jobject startObj = JavaNode::get( env, const_cast<Node*>( &transition->start ) );
	jobject endObj = JavaNode::get( env, const_cast<Node*>( &transition->end ) );

	switch ( transition->type ) {
	case Transition::WORD: {
		Word * word = dynamic_cast<Word*>( transition.get() );
		object = env->NewWeakGlobalRef( env->NewObject(
				wordClazz, wordConstructor, (jint)transition->id,
				textObj, startObj, endObj, // Текст и узлы
				out( env, word->getBase() ), // Начальная форма слова
				out( env, word->getToken() ), // Слово
				word->getSpeechPart().id ) ); // Часть речи
		break;
	}
	case Transition::MATCH: {
		Match * match = dynamic_cast<Match*>( transition.get() );
		jobject patternObj = lspl::patterns::java::JavaPattern::get( env, const_cast<Pattern*>( &match->getPattern() ) ).object;
		object = env->NewWeakGlobalRef( env->NewObject(
				matchClazz, matchConstructor, (jint)transition->id,
				textObj, startObj, endObj, // Текст и узлы
				patternObj ) ); // Шаблон
		break;
	}
	case Transition::TOKEN: {
		Token * token = dynamic_cast<Token*>( transition.get() );
		object = env->NewWeakGlobalRef( env->NewObject(
				punctuationClazz, punctuationConstructor, (jint)transition->id,
				textObj, startObj, endObj, // Текст и узлы
				out( env, token->getToken() ) ) ); // Шаблон
		break;
	}
	default:
		throw lspl::base::Exception( "Unknown pattern type", __FILE__, __LINE__);
	}
}

JavaTransition::~JavaTransition() {
}

TransitionRef JavaTransition::get( JNIEnv * env, jobject obj ) {
	return transitions.at( env->GetIntField( obj, transitionIdField ) )->transition;
}

jobject JavaTransition::get( JNIEnv * env, Transition * transition ) {
	if ( transitions.size() <= transition->id )
		transitions.resize( transition->id + 1 );

	JavaTransition * result = transitions[ transition->id ];

	if ( !result ) {
		result = transitions[ transition->id ] = new JavaTransition( transition, env );
	}

	return result->object;
}


void JavaTransition::init( JNIEnv * env ) {
	transitionClazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/text/Transition" ) );
	transitionIdField = env->GetFieldID( transitionClazz, "id", "I" );
	transitionConstructor = env->GetMethodID( transitionClazz, "<init>", "(ILru/lspl/text/Text;Lru/lspl/text/Node;Lru/lspl/text/Node;)V" );

	wordClazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/text/Word" ) );
	wordConstructor = env->GetMethodID( wordClazz, "<init>", "(ILru/lspl/text/Text;Lru/lspl/text/Node;Lru/lspl/text/Node;Ljava/lang/String;Ljava/lang/String;I)V" );

	matchClazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/text/Match" ) );
	matchConstructor = env->GetMethodID( matchClazz, "<init>", "(ILru/lspl/text/Text;Lru/lspl/text/Node;Lru/lspl/text/Node;Lru/lspl/patterns/Pattern;)V" );

	punctuationClazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/text/Token" ) );
	punctuationConstructor = env->GetMethodID( punctuationClazz, "<init>", "(ILru/lspl/text/Text;Lru/lspl/text/Node;Lru/lspl/text/Node;Ljava/lang/String;)V" );
}

void JavaTransition::remove( JNIEnv * env, jobject obj ) {
	uint id = env->GetIntField( obj, transitionIdField );

	transitions[ id ] = 0;

	delete transitions.at( id );
}

} } }
