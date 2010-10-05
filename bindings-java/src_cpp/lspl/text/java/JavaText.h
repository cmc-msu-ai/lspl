#ifndef _LSPL_JAVA_JAVATEXT_H_
#define _LSPL_JAVA_JAVATEXT_H_

#include <jni.h>
#include <vector>

#include "lspl/base/Base.h"
#include "lspl/text/Forward.h"

namespace lspl { namespace text { namespace java {

class JavaText {
public:
	JavaText( const TextRef & text, JNIEnv * env, jstring content);
	JavaText( const TextRef & text, jobject object );
	~JavaText();

	static void init( JNIEnv * env );
	static void remove( JNIEnv * env, jobject obj );
	static JavaText & get( JNIEnv * env, jobject obj );
	static JavaText & get( JNIEnv * env, const Text & text );
	static JavaText & create( JNIEnv * env, jstring content );
	static JavaText & create( JNIEnv * env, jstring content, const TextConfig & config );
	
	static Node & getNode( JNIEnv * env, jobject obj );
	static Transition & getTransition( JNIEnv * env, jobject obj );

	static jobjectArray createTextNodeArray( JNIEnv * env, jobject obj );
private:

	jobjectArray createNodeArray( JNIEnv * env );

public:
	TextRef text;
	jobject object;
private:
	static jclass 	clazz;
	static jfieldID idField;
	static jmethodID constructor, initializer;

	static jclass 	nodeClazz;
	static jfieldID nodeIndexField, nodeTextField;
	static jmethodID nodeConstructor;

	static jclass 	transitionClazz;
	static jfieldID transitionIndexField, transitionNodeIndexField, transitionTextField;
	static jmethodID transitionConstructor;

	static std::vector<JavaText *> texts;
};

} } }

#endif /*_LSPL_JAVA_JAVATEXT_H_*/
