#ifndef _LSPL_TEXT_JAVALOOPITERATION_H_
#define _LSPL_TEXT_JAVALOOPITERATION_H_

#include "JavaTransition.h"

namespace lspl { namespace text {

class JavaLoopIteration : public JavaTransition {
public:
	JavaLoopIteration( Transition * transition, JNIEnv * env );
	virtual ~JavaLoopIteration();

	static JavaLoopIteration * get( JNIEnv * env, jobject obj ) { return static_cast<JavaLoopIteration*>( JavaTransition::get(env,obj) ); }
	static JavaLoopIteration * get( JNIEnv * env, const Transition * match ) { return static_cast<JavaLoopIteration*>( JavaTransition::get(env,match) ); }
	static void init( JNIEnv * env );

public:
	static jclass	 clazz, variantClazz;
	static jmethodID constructor, variantConstructor;
};

} }

#endif /*_LSPL_TEXT_JAVALOOPITERATION_H_*/
