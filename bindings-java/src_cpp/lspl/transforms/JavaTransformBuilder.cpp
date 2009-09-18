/*
 * JavaTransformBuilder.cpp
 *
 *  Created on: Sep 18, 2009
 *      Author: alno
 */

#include "JavaTransformBuilder.h"
#include "JavaTransform.h"

#include "../java/Utils.h"
#include "../java/JavaAlternative.h"

#include "lspl/patterns/Alternative.h"

using namespace lspl::java;

using lspl::patterns::java::JavaAlternative;

namespace lspl { namespace transforms {

jclass JavaTransformBuilder::clazz;
jmethodID JavaTransformBuilder::buildMethod;

JavaTransformBuilder::JavaTransformBuilder( JNIEnv * env, jobject obj ) {
	object = env->NewGlobalRef( obj );
}

JavaTransformBuilder::~JavaTransformBuilder() {
	getCurrentEnv()->DeleteGlobalRef( object );
}

void JavaTransformBuilder::init( JNIEnv * env ) {
	clazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/transforms/TransformBuilder" ) );
	buildMethod = env->GetMethodID( clazz, "build", "(Lru/lspl/patterns/Alternative;Ljava/lang/String;)Lru/lspl/transforms/Transform;" );
}

TypedTransform<jobject> * JavaTransformBuilder::build( const patterns::Alternative & alternative, const std::string & source ) {
	JNIEnv * env = getCurrentEnv();
	jobject transform = env->CallObjectMethod( const_cast<jobject>( object ), buildMethod, JavaAlternative::get( env, alternative.pattern, &alternative ).object, out( env, source ) );

	return transform ? new JavaTransform( env, transform ) : 0;
}

} } // namespace lspl::transforms
