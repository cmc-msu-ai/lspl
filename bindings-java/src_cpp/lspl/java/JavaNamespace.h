#ifndef _LSPL_JAVA_JAVANAMESPACE_H_
#define _LSPL_JAVA_JAVANAMESPACE_H_

#include <jni.h>
#include <vector>

#include "lspl/base/Base.h"
#include "lspl/Namespace.h"

namespace lspl { namespace java {

class JavaNamespace {
public:
	JavaNamespace( Namespace * ns, JNIEnv * env );
	virtual ~JavaNamespace();

public:

	static JavaNamespace & get( JNIEnv * env, jobject obj );
	static JavaNamespace & get( JNIEnv * env, Namespace * builder );
	static void init( JNIEnv * env );
	static void remove( JNIEnv * env, jobject obj );

public:
	NamespaceRef ns;
	jobject object;
private:
	static jclass 	clazz;
	static jfieldID idField;
	static jmethodID constructor;

	static std::vector<JavaNamespace*> namespaces;
};

} }

#endif /*_LSPL_JAVA_JAVANAMESPACE_H_*/
