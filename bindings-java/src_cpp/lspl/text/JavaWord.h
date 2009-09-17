#ifndef _LSPL_TEXT_JAVAWORD_H_
#define _LSPL_TEXT_JAVAWORD_H_

#include "JavaTransition.h"

namespace lspl { namespace text {

class JavaWord : public JavaTransition {
public:
	JavaWord( Transition * transition, JNIEnv * env );
	virtual ~JavaWord();

	static void init( JNIEnv * env );

private:
	static jclass	 clazz;
	static jmethodID constructor;
};

} }

#endif /*_LSPL_TEXT_JAVAWORD_H_*/
