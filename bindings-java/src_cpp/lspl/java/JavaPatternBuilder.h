#ifndef _LSPL_JAVA_JAVAPATTERNBUILDER_H_
#define _LSPL_JAVA_JAVAPATTERNBUILDER_H_

#include <jni.h>
#include <vector>

#include "lspl/base/Base.h"
#include "lspl/patterns/Forward.h"

namespace lspl { namespace patterns { namespace java {

class JavaPattern;

class JavaPatternBuilder {
public:
	JavaPatternBuilder( PatternBuilder * builder, JNIEnv * env );
	virtual ~JavaPatternBuilder();

public:

	static JavaPatternBuilder & get( JNIEnv * env, jobject obj );
	static JavaPatternBuilder & get( JNIEnv * env, PatternBuilder * builder );
	static void init( JNIEnv * env );
	static void remove( JNIEnv * env, jobject obj );

public:
	PatternBuilderRef builder;
	jobject object;
private:
	static jclass 	clazz;
	static jfieldID idField;
	static jmethodID constructor;

	static std::vector<JavaPatternBuilder*> builders;
};

} } }

#endif /*_LSPL_JAVA_JAVAPATTERNBUILDER_H_*/
