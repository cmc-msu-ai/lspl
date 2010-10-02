#include "JavaPattern.h"
#include "Utils.h"

#include "lspl/patterns/Pattern.h"

using namespace lspl::java;

namespace lspl { namespace patterns { namespace java {

jclass JavaPattern::clazz;
jfieldID JavaPattern::idField;
jmethodID JavaPattern::constructor;

std::vector<JavaPattern*> JavaPattern::patterns;

JavaPattern::JavaPattern( PatternRef pattern, jobject object ) : pattern( pattern ), object( object ) {
}

JavaPattern::JavaPattern( PatternRef pattern, JNIEnv * env ) : pattern( pattern ),
	object( env->NewWeakGlobalRef( env->NewObject( clazz, constructor, (jint)pattern->id, out( env, pattern->name ) ) ) ) {
}

JavaPattern::~JavaPattern() {
}

JavaPattern & JavaPattern::get( JNIEnv * env, jobject obj ) {
	return *patterns.at( env->GetIntField( obj, idField ) );
}

JavaPattern & JavaPattern::get( JNIEnv * env, const Pattern * pattern ) {
	if ( patterns.size() <= pattern->id )
		patterns.resize( pattern->id + 1 );

	JavaPattern * result = patterns[ pattern->id ];

	if ( result ) return *result;

	return *(result = patterns[ pattern->id ] = new JavaPattern( const_cast<Pattern*>( pattern ), env ) );
}

void JavaPattern::init( JNIEnv * env ) {
	clazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/patterns/Pattern" ) );
	idField = env->GetFieldID( clazz, "id", "I" );
	constructor = env->GetMethodID( clazz, "<init>", "(ILjava/lang/String;)V" );
}

void JavaPattern::remove( JNIEnv * env, jobject obj ) {
	uint id = env->GetIntField( obj, idField );

	delete patterns.at( id );
	patterns[ id ] = 0;
}

} } }
