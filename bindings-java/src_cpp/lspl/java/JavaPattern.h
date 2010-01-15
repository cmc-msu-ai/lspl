#ifndef _LSPL_JAVA_JAVAPATTERN_H_
#define _LSPL_JAVA_JAVAPATTERN_H_

#include <jni.h>
#include <vector>

#include "lspl/base/Base.h"
#include "lspl/patterns/Forward.h"

namespace lspl { namespace patterns { namespace java {

class JavaAlternative;

class JavaPattern {
public:
	JavaPattern( PatternRef pattern, jobject object );
	JavaPattern( PatternRef pattern, JNIEnv * env );
	virtual ~JavaPattern();

	static JavaPattern & get( JNIEnv * env, jobject obj );
	static JavaPattern & get( JNIEnv * env, const patterns::Pattern * pattern );
	static void init( JNIEnv * env );
	static void remove( JNIEnv * env, jobject obj );

	Pattern * operator ->() {
		return pattern.get();
	}

public:
	PatternRef pattern;
	jobject object;

	std::vector<JavaAlternative*> alternatives;
private:
	static jclass 	clazz;
	static jfieldID idField;
	static jmethodID constructor;

	static std::vector<JavaPattern*> patterns;
};

} } }

#endif /*_LSPL_JAVA_JAVAPATTERN_H_*/
