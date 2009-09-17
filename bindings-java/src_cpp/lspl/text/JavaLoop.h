#ifndef _LSPL_TEXT_JAVALOOP_H_
#define _LSPL_TEXT_JAVALOOP_H_

#include "JavaTransition.h"

namespace lspl { namespace text {

class JavaLoop : public JavaTransition {
public:
	JavaLoop( Transition * transition, JNIEnv * env );
	virtual ~JavaLoop();

	static void init( JNIEnv * env );

private:
	static jclass	 clazz;
	static jmethodID constructor;
};

} }

#endif /*_LSPL_TEXT_JAVALOOP_H_*/
