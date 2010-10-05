#ifndef _LSPL_TEXT_JAVAMATCH_H_
#define _LSPL_TEXT_JAVAMATCH_H_

#include "JavaTransition.h"

namespace lspl { namespace text {

class JavaMatch : public JavaTransition {
public:
	JavaMatch( Transition * transition, JNIEnv * env );
	virtual ~JavaMatch();

	static JavaMatch * get( JNIEnv * env, jobject obj ) { return static_cast<JavaMatch*>( JavaTransition::get(env,obj) ); }
	static JavaMatch * get( JNIEnv * env, const Transition * match ) { return static_cast<JavaMatch*>( JavaTransition::get(env,match) ); }
	static void init( JNIEnv * env );

public:
	static jclass	 clazz, variantClazz;
	static jmethodID constructor, variantConstructor, getVariantMethod;
};

} }

#endif /*_LSPL_TEXT_JAVAMATCH_H_*/
