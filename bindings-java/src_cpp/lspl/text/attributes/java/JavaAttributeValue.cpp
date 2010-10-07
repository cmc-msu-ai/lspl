#include "JavaAttributeValue.h"

#include "lspl/java/Utils.h"
#include <iostream>

using namespace lspl::java;

namespace lspl { namespace text { namespace attributes { namespace java {

jclass JavaAttributeValue::indexedAttributeClazz;
jmethodID JavaAttributeValue::indexedAttributeCreator;

void JavaAttributeValue::init( JNIEnv * env ) {
	indexedAttributeClazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/text/attributes/IndexedAttribute" ) );
	indexedAttributeCreator = env->GetStaticMethodID( indexedAttributeClazz, "valueOf", "(I)Lru/lspl/text/attributes/IndexedAttribute;" );
}

jobject JavaAttributeValue::get( JNIEnv * env, const AttributeValue & value ) {
	if ( value == AttributeValue::UNDEFINED ) {
		return 0;
	} else if ( value.type == AttributeType::INDEXED ) {
		return env->CallStaticObjectMethod( indexedAttributeClazz, indexedAttributeCreator, value.value );
	} else if ( value.type == AttributeType::STRING ) {
		return out( env, *value.getStringPtr() );
	} else if ( value.type == AttributeType::COMPOUND ) {
		return 0; // TODO
	}
	
	return 0;
}

} } } }
