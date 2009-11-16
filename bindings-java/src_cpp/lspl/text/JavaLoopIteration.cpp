#include "JavaLoopIteration.h"

#include "java/JavaNode.h"
#include "java/JavaText.h"

#include "lspl/java/Utils.h"
#include "lspl/java/JavaPattern.h"

#include "lspl/patterns/Pattern.h"

#include "lspl/text/Text.h"
#include "lspl/text/Node.h"
#include "lspl/text/Transition.h"
#include "lspl/text/Loop.h"

#include <iostream>

using lspl::java::out;

using lspl::patterns::Pattern;
using lspl::patterns::java::JavaPattern;

using namespace lspl::text::java;

#include <iostream>

namespace lspl { namespace text {

jclass JavaLoopIteration::clazz;
jmethodID JavaLoopIteration::constructor;

jclass JavaLoopIteration::variantClazz;
jmethodID JavaLoopIteration::variantConstructor;

JavaLoopIteration::JavaLoopIteration( Transition * t, JNIEnv * env ) {
	transition = t;
	object = env->NewWeakGlobalRef( env->NewObject(
			clazz, constructor, (jint)transition->id,
			JavaText::get( env, const_cast<Text*>( &transition->start.text ) ).object,
			JavaNode::get( env, const_cast<Node*>( &transition->start ) ),
			JavaNode::get( env, const_cast<Node*>( &transition->end ) ) ) ); // Шаблон
}

JavaLoopIteration::~JavaLoopIteration() {
}

void JavaLoopIteration::init( JNIEnv * env ) {
	clazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/text/LoopIteration" ) );
	constructor = env->GetMethodID( clazz, "<init>", "(ILru/lspl/text/Text;Lru/lspl/text/Node;Lru/lspl/text/Node;)V" );

	variantClazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/text/LoopIterationVariant" ) );
	variantConstructor = env->GetMethodID( variantClazz, "<init>", "(Lru/lspl/text/LoopIteration;I)V" );
}

jobject JavaLoopIteration::getVariant( JNIEnv * env, int index ) {
	if ( index >= variants.size() )
		variants.resize( index + 1, 0 );

	jobject obj = variants[ index ];

	if ( obj )
		return obj;

	obj = env->NewWeakGlobalRef( env->NewObject(
			variantClazz, variantConstructor,
			object,
			index ) );

	variants[index] = obj;

	return obj;
}

void JavaLoopIteration::freeVariant( int index ) {
	if ( index >= variants.size() || variants[index] == 0 )
		return;

	variants[index] = 0;
}

} }
