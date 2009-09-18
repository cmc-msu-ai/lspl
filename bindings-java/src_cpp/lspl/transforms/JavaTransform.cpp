/*
 * JavaTransform.cpp
 *
 *  Created on: Sep 18, 2009
 *      Author: alno
 */

#include "JavaTransform.h"

#include "../java/Utils.h"
#include "../text/JavaMatch.h"

#include "lspl/text/Match.h"

using namespace lspl::java;

using lspl::text::JavaMatch;

namespace lspl { namespace transforms {

jclass JavaTransform::clazz;
jmethodID JavaTransform::applyMethod;

JavaTransform::JavaTransform( JNIEnv * env, jobject obj ) {
	object = env->NewGlobalRef( obj );
}

JavaTransform::~JavaTransform() {
	getCurrentEnv()->DeleteGlobalRef( object );
}

void JavaTransform::init( JNIEnv * env ) {
	clazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/transforms/Transform" ) );
	applyMethod = env->GetMethodID( clazz, "apply", "(Lru/lspl/text/MatchVariant;)Ljava/lang/Object;" );
}

TypedTransformResult<jobject> * JavaTransform::apply( const text::MatchVariant & matchVariant ) const {
	JNIEnv * env = getCurrentEnv();

	return new TypedTransformResult<jobject>( env->CallObjectMethod( const_cast<jobject>( object ), applyMethod, JavaMatch::get( env, matchVariant.match )->getVariant( env, matchVariant.index ) ) );
}

} } // namespace lspl::transforms
