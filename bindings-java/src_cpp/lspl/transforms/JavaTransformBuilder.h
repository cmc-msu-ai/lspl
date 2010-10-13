/*
 * JavaTransformBuilder.h
 *
 *  Created on: Sep 18, 2009
 *      Author: alno
 */

#ifndef _LSPL_TRANSFORMS_JAVATRANSFORMBUILDER_H_
#define _LSPL_TRANSFORMS_JAVATRANSFORMBUILDER_H_

#include "lspl/base/Base.h"
#include "lspl/transforms/TransformBuilder.h"

#include <jni.h>

namespace lspl { namespace transforms {

class JavaTransformBuilder : public TransformBuilder {
public:
	JavaTransformBuilder( JNIEnv * env, jobject obj );
	virtual ~JavaTransformBuilder();

	virtual TypedTransform<jobject> * build( const patterns::Alternative & alternative, const std::string & source );

	static void init( JNIEnv * env );

private:
	static jclass  clazz;
	static jmethodID buildMethod;

	jobject object;
};

} } // namespace lspl::transforms

#endif /* _LSPL_TRANSFORMS_JAVATRANSFORMBUILDER_H_ */
