#include "JavaToken.h"

#include "java/JavaText.h"

#include "lspl/java/Utils.h"
#include "lspl/java/JavaPattern.h"

#include "lspl/patterns/Pattern.h"

#include "lspl/text/Text.h"
#include "lspl/text/Node.h"
#include "lspl/text/Transition.h"
#include "lspl/text/markup/Token.h"

#include <iostream>

using lspl::java::out;

using namespace lspl::text::markup;
using namespace lspl::text::java;

namespace lspl { namespace text {

jclass JavaToken::clazz;
jmethodID JavaToken::constructor;

JavaToken::JavaToken( Transition * t, JNIEnv * env ) {
	Token * token = dynamic_cast<Token*>( t );

	transition = t;
	object = env->NewWeakGlobalRef( env->NewObject(
			clazz, constructor, (jint)transition->id,
			JavaText::get( env, transition->start.text ).object, // Текст
			(jint)transition->start.index, // Start node index
			(jint)transition->end.index ) ); // End node index
}

JavaToken::~JavaToken() {
}

void JavaToken::init( JNIEnv * env ) {
	clazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/text/Token" ) );
	constructor = env->GetMethodID( clazz, "<init>", "(ILru/lspl/text/Text;II)V" );
}

} }
