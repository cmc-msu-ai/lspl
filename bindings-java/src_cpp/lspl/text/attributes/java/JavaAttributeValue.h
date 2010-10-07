#ifndef JAVAATTRIBUTEVALUE_H_
#define JAVAATTRIBUTEVALUE_H_

#include "lspl/text/attributes/AttributeValue.h"

#include <jni.h>

namespace lspl { namespace text { namespace attributes { namespace java {

class JavaAttributeValue {	
public:
	static void init( JNIEnv * env );
	
	static jobject get( JNIEnv * env, const AttributeValue & value );
private:
	static jclass 	indexedAttributeClazz;
	static jmethodID indexedAttributeCreator;
};

} } } }

#endif /*JAVAATTRIBUTEVALUE_H_*/
