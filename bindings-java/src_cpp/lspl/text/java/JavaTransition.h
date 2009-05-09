#ifndef _LSPL_TEXT_JAVA_JAVATRANSITION_H_
#define _LSPL_TEXT_JAVA_JAVATRANSITION_H_

#include <jni.h>
#include <vector>

#include "lspl/base/Base.h"
#include "lspl/base/RefCountPtr.h"
#include "lspl/text/Forward.h"

namespace lspl { namespace text { namespace java {

class JavaTransition {
public:
	JavaTransition( TransitionRef transition, JNIEnv * env );
	virtual ~JavaTransition();

	static void init( JNIEnv * env );
	static void remove( JNIEnv * env, jobject obj );
	static TransitionRef get( JNIEnv * env, jobject obj );
	static jobject get( JNIEnv * env, Transition * transition );

public:
	TransitionRef transition;
	jobject object;
private:
	static jclass 	 transitionClazz;
	static jmethodID transitionConstructor;
	static jfieldID  transitionIdField;

	static jclass	 wordClazz;
	static jmethodID wordConstructor;

	static jclass	 matchClazz;
	static jmethodID matchConstructor;

	static jclass	 punctuationClazz;
	static jmethodID punctuationConstructor;

	static std::vector<JavaTransition *> transitions;
};

} } }

#endif /*_LSPL_TEXT_JAVA_JAVATRANSITION_H_*/
