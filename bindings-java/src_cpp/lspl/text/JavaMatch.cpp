#include "JavaMatch.h"

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
jmethodID JavaMatch::getVariantMethod;

jclass JavaMatch::variantClazz;
jmethodID JavaMatch::variantConstructor;

JavaMatch::JavaMatch( Transition * t, JNIEnv * env ) {
	Match * match = dynamic_cast<Match*>( t );

	transition = t;
	object = env->NewWeakGlobalRef( env->NewObject(
			clazz, constructor, (jint)transition->id,
			JavaText::get( env, transition->start.text ).object,
			(jint)transition->start.index, // Start node index
			(jint)transition->end.index, // End node index
			JavaPattern::get( env, const_cast<Pattern*>( &match->getPattern() ) ).object ) ); // Шаблон
}

JavaMatch::~JavaMatch() {
}

void JavaMatch::init( JNIEnv * env ) {
	clazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/text/Match" ) );
	constructor = env->GetMethodID( clazz, "<init>", "(ILru/lspl/text/Text;IILru/lspl/patterns/Pattern;)V" );
	getVariantMethod = env->GetMethodID( clazz, "getVariant", "(I)Lru/lspl/text/MatchVariant;" );

	variantClazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/text/MatchVariant" ) );
	variantConstructor = env->GetMethodID( variantClazz, "<init>", "(Lru/lspl/text/Match;I)V" );
}

} }
