#include "JavaAlternative.h"
#include "Utils.h"

#include "lspl/patterns/Alternative.h"
#include "lspl/patterns/Pattern.h"

#include "lspl/java/JavaPattern.h"

using namespace lspl::patterns;

namespace lspl { namespace patterns { namespace java {

jclass JavaAlternative::clazz;
jfieldID JavaAlternative::idField;
jfieldID JavaAlternative::patternField;
jmethodID JavaAlternative::constructor;

static uint findIndex( const Pattern * pattern, const Alternative * alternative ) {
	for ( uint i = 0; i < pattern->alternatives.size(); ++ i )
		if ( &pattern->alternatives[i] == alternative )
			return i;

	throw std::logic_error( "Alternative not in pattern" );
}

JavaAlternative::JavaAlternative(Pattern * pattern, Alternative * alt, JNIEnv * env ) : alternative( alt ),
	object( env->NewGlobalRef( env->NewObject( clazz, constructor, (jint)findIndex( pattern, alt ), JavaPattern::get( env, pattern ).object ) ) ) {
}

JavaAlternative::~JavaAlternative() {
}

JavaAlternative & JavaAlternative::get( JNIEnv * env, jobject obj ) {
	return *JavaPattern::get( env, env->GetObjectField( obj, patternField ) ).alternatives.at( env->GetIntField( obj, idField ) );
}

JavaAlternative & JavaAlternative::get( JNIEnv * env, const Pattern * patternPtr, const Alternative * alternative ) {
	JavaPattern & pattern = JavaPattern::get( env, patternPtr );
	uint index = findIndex( patternPtr, alternative );

	if ( pattern.alternatives.size() <= index )
		pattern.alternatives.resize( index + 1 );

	JavaAlternative * result = pattern.alternatives[ index ];

	if ( result ) return *result;

	return *(result = pattern.alternatives[ index ] = new JavaAlternative( const_cast<Pattern*>( patternPtr ), const_cast<Alternative*>( alternative ), env ) );
}

void JavaAlternative::init( JNIEnv * env ) {
	clazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/patterns/Alternative" ) );
	idField = env->GetFieldID( clazz, "id", "I" );
	patternField = env->GetFieldID( clazz, "pattern", "Lru/lspl/patterns/Pattern;" );
	constructor = env->GetMethodID( clazz, "<init>", "(ILru/lspl/patterns/Pattern;)V" );
}

void JavaAlternative::remove( JNIEnv * env, jobject obj ) {
	JavaPattern & pattern = JavaPattern::get( env, env->GetObjectField( obj, patternField ) );
	uint id = env->GetIntField( obj, idField );

	delete pattern.alternatives.at( id );
	pattern.alternatives[ id ] = 0;
}

} } }
