#ifndef _LSPL_TEXT_JAVAMATCH_H_
#define _LSPL_TEXT_JAVAMATCH_H_

#include "JavaTransition.h"

namespace lspl { namespace text {

class JavaMatch : public JavaTransition {
public:
	JavaMatch( Transition * transition, JNIEnv * env );
	virtual ~JavaMatch();

	static void init( JNIEnv * env );

private:
	static jclass	 clazz;
	static jmethodID constructor;
};

} }

#endif /*_LSPL_TEXT_JAVAMATCH_H_*/
