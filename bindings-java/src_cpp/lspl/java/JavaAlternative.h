#ifndef _LSPL_JAVA_JAVAALTERNATIVE_H_
#define _LSPL_JAVA_JAVAALTERNATIVE_H_

#include <jni.h>
#include <vector>

#include "lspl/base/Base.h"
#include "lspl/base/RefCountPtr.h"
#include "lspl/patterns/Forward.h"

namespace lspl { namespace patterns { namespace java {

class JavaAlternative {
public:
	JavaAlternative( patterns::Pattern * pattern, patterns::Alternative * alternative, JNIEnv * env );
	virtual ~JavaAlternative();

	static JavaAlternative & get( JNIEnv * env, jobject obj );
	static JavaAlternative & get( JNIEnv * env, const patterns::Pattern * pattern, const patterns::Alternative * alternative );
	static void init( JNIEnv * env );
	static void remove( JNIEnv * env, jobject obj );

	patterns::Alternative * operator ->() {
		return alternative;
	}

public:
	patterns::Alternative * alternative;
	jobject object;

private:
	static jclass 	clazz;
	static jfieldID idField;
	static jfieldID patternField;
	static jmethodID constructor;
};

} } }

#endif /*_LSPL_JAVA_JAVAALTERNATIVE_H_*/
