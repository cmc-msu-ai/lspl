#include "JavaNamespace.h"

#include "Utils.h"

namespace lspl { namespace java {

jclass JavaNamespace::clazz;
jfieldID JavaNamespace::idField;
jmethodID JavaNamespace::constructor;

std::vector<JavaNamespace*> JavaNamespace::namespaces;

JavaNamespace::JavaNamespace( Namespace * ns, JNIEnv * env ) :
	ns( ns ),
	object( env->NewWeakGlobalRef( env->NewObject( clazz, constructor, (jint)ns->id ) ) ) {
}

JavaNamespace::~JavaNamespace() {
}

JavaNamespace & JavaNamespace::get( JNIEnv * env, jobject obj ) {
	return *namespaces.at( env->GetIntField( obj, idField ) );
}

JavaNamespace & JavaNamespace::get( JNIEnv * env, Namespace * ns ) {
	if ( namespaces.size() <= ns->id )
		namespaces.resize( ns->id + 1 );

	JavaNamespace * result = namespaces[ ns->id ];

	if ( result ) return *result;

	return *(result = namespaces[ ns->id ] = new JavaNamespace( ns, env ) );
}

void JavaNamespace::init( JNIEnv * env ) {
	clazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/Namespace" ) );
	idField = env->GetFieldID( clazz, "id", "I" );
	constructor = env->GetMethodID( clazz, "<init>", "(I)V" );
}

void JavaNamespace::remove( JNIEnv * env, jobject obj ) {
	uint id = env->GetIntField( obj, idField );

	delete namespaces.at( id );
	namespaces[ id ] = 0;
}

} }
