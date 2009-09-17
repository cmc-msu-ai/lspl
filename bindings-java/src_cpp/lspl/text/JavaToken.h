#ifndef _LSPL_TEXT_JAVATOKEN_H_
#define _LSPL_TEXT_JAVATOKEN_H_

#include "JavaTransition.h"

namespace lspl { namespace text {

class JavaToken : public JavaTransition {
public:
	JavaToken( Transition * transition, JNIEnv * env );
	virtual ~JavaToken();

	static void init( JNIEnv * env );

private:
	static jclass	 clazz;
	static jmethodID constructor;
};

} }

#endif /*_LSPL_TEXT_JAVATOKEN_H_*/
