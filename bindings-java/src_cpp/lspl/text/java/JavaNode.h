#ifndef _LSPL_TEXT_JAVA_JAVANODE_H_
#define _LSPL_TEXT_JAVA_JAVANODE_H_

#include <jni.h>
#include <vector>

#include "lspl/base/Base.h"
#include "lspl/text/Forward.h"

namespace lspl { namespace text { namespace java {

class JavaNode {
public:
	JavaNode( NodeRef node, JNIEnv * env );
	virtual ~JavaNode();

	static void init( JNIEnv * env );
	static void remove( JNIEnv * env, jobject obj );
	static NodeRef get( JNIEnv * env, jobject obj );
	static jobject get( JNIEnv * env, Node * node );

public:
	NodeRef node;
	jobject object;
	
public:
	static jclass 	clazz;
	static jfieldID idField;
	static jmethodID constructor;

private:
	static std::vector<JavaNode *> nodes;
};

} } }

#endif /*_LSPL_TEXT_JAVA_JAVANODE_H_*/
