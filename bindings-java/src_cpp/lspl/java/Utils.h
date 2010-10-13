#ifndef _LSPL_JAVA_UTILS_H_
#define _LSPL_JAVA_UTILS_H_

#include <jni.h>

#include <string>

namespace lspl { namespace java {

std::string in( JNIEnv * env, jstring str );
jstring out( JNIEnv * env, const std::string & str );
jstring out( JNIEnv * env, const char * str );

JNIEnv * getCurrentEnv();

void setupVM( JNIEnv * env );

void throwRuntimeException( JNIEnv * env, const char * text );

} }

#endif /*_LSPL_JAVA_UTILS_H_*/
