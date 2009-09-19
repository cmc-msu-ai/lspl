/*
 * JavaTransform.h
 *
 *  Created on: Sep 18, 2009
 *      Author: alno
 */

#ifndef _LSPL_TRANSFORMS_JAVATRANSFORM_H_
#define _LSPL_TRANSFORMS_JAVATRANSFORM_H_

#include "lspl/base/Base.h"
#include "lspl/transforms/Transform.h"
#include "lspl/transforms/TransformResult.h"

#include <jni.h>

namespace lspl { namespace transforms {

class JavaTransformResult : public TypedTransformResult<jobject> {
public:
	JavaTransformResult( JNIEnv * env, jobject value );
	virtual ~JavaTransformResult();
};

class JavaTransform : public TypedTransform<jobject> {
public:
	JavaTransform( JNIEnv * env, jobject obj );
	virtual ~JavaTransform();

	virtual JavaTransformResult * apply( const text::MatchVariant & matchVariant ) const;

	static void init( JNIEnv * env );

private:
	static jclass  clazz;
	static jmethodID applyMethod;

	jobject object;
};

} } // namespace lspl::transforms

#endif /* _LSPL_TRANSFORMS_JAVATRANSFORM_H_ */
