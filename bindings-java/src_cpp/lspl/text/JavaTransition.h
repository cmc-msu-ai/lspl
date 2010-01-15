#ifndef _LSPL_TEXT_JAVATRANSITION_H_
#define _LSPL_TEXT_JAVATRANSITION_H_

#include <jni.h>
#include <vector>

#include "lspl/base/Base.h"
#include "lspl/text/Forward.h"

namespace lspl { namespace text {

class JavaTransition {
public:
	virtual ~JavaTransition();

	static void init( JNIEnv * env );
	static void remove( JNIEnv * env, jobject obj );
	static JavaTransition * get( JNIEnv * env, jobject obj );
	static JavaTransition * get( JNIEnv * env, const Transition * transition );

private:

	static JavaTransition * create( Transition * transition, JNIEnv * env );

public:
	TransitionRef transition;
	jobject object;
protected:
	static jclass 	 transitionClazz;
	static jmethodID transitionConstructor;
	static jfieldID  transitionIdField;

	static std::vector<JavaTransition *> transitions;
};

} }

#endif /*_LSPL_TEXT_JAVATRANSITION_H_*/
