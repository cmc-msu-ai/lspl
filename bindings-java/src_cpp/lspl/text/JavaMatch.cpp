#include "JavaMatch.h"

#include "java/JavaNode.h"
#include "java/JavaText.h"

#include "lspl/java/Utils.h"
#include "lspl/java/JavaPattern.h"

#include "lspl/patterns/Pattern.h"

#include "lspl/text/Text.h"
#include "lspl/text/Node.h"
#include "lspl/text/Transition.h"
#include "lspl/text/Match.h"

#include <iostream>

using lspl::java::out;

using lspl::patterns::Pattern;
using lspl::patterns::java::JavaPattern;

using namespace lspl::text::java;

#include <iostream>

namespace lspl { namespace text {

jclass JavaMatch::clazz;
jmethodID JavaMatch::constructor;

JavaMatch::JavaMatch( Transition * t, JNIEnv * env ) {
	Match * match = dynamic_cast<Match*>( t );

	transition = t;
	object = env->NewWeakGlobalRef( env->NewObject(
			clazz, constructor, (jint)transition->id,
			JavaText::get( env, const_cast<Text*>( &transition->start.text ) ).object,
			JavaNode::get( env, const_cast<Node*>( &transition->start ) ),
			JavaNode::get( env, const_cast<Node*>( &transition->end ) ), // Текст и узлы
			JavaPattern::get( env, const_cast<Pattern*>( &match->getPattern() ) ).object ) ); // Шаблон
}

JavaMatch::~JavaMatch() {
}

void JavaMatch::init( JNIEnv * env ) {
	clazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/text/Match" ) );
	constructor = env->GetMethodID( clazz, "<init>", "(ILru/lspl/text/Text;Lru/lspl/text/Node;Lru/lspl/text/Node;Lru/lspl/patterns/Pattern;)V" );
}

} }
