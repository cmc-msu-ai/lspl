#include "JavaLoop.h"

#include "java/JavaText.h"

#include "lspl/java/Utils.h"

#include "lspl/text/Text.h"
#include "lspl/text/Node.h"
#include "lspl/text/Transition.h"
#include "lspl/text/Loop.h"

#include <iostream>

using namespace lspl::text::java;

namespace lspl { namespace text {

jclass JavaLoop::clazz;
jmethodID JavaLoop::constructor;

JavaLoop::JavaLoop( Transition * t, JNIEnv * env ) {
	transition = t;
	object = env->NewWeakGlobalRef( env->NewObject(
			clazz, constructor, (jint)transition->id,
			JavaText::get( env, transition->start.text ).object, // Текст
			JavaText::getNode( env, transition->start ), // Начальный узел
			JavaText::getNode( env, transition->end ) ) ); // Конечный узел
}

JavaLoop::~JavaLoop() {
}

void JavaLoop::init( JNIEnv * env ) {
	clazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/text/Loop" ) );
	constructor = env->GetMethodID( clazz, "<init>", "(ILru/lspl/text/Text;Lru/lspl/text/Node;Lru/lspl/text/Node;)V" );
}

} }
