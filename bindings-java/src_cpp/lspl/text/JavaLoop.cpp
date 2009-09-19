#include "JavaLoop.h"

#include "java/JavaNode.h"
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
	Loop * loop = dynamic_cast<Loop*>( t );

	transition = t;
	object = env->NewWeakGlobalRef( env->NewObject(
			clazz, constructor, (jint)transition->id,
			JavaText::get( env, const_cast<Text*>( &transition->start.text ) ).object, // Текст
			JavaNode::get( env, const_cast<Node*>( &transition->start ) ), // Начальный узел
			JavaNode::get( env, const_cast<Node*>( &transition->end ) ),
			loop->repeatCount ) ); // Конечный узел
}

JavaLoop::~JavaLoop() {
}

void JavaLoop::init( JNIEnv * env ) {
	clazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/text/Loop" ) );
	constructor = env->GetMethodID( clazz, "<init>", "(ILru/lspl/text/Text;Lru/lspl/text/Node;Lru/lspl/text/Node;I)V" );
}

} }
