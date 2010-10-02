#include "JavaPatternBuilder.h"
#include "JavaPattern.h"

#include "lspl/patterns/PatternBuilder.h"
#include "lspl/patterns/Pattern.h"

#include "Utils.h"

using namespace lspl::patterns;

namespace lspl { namespace patterns { namespace java {

jclass JavaPatternBuilder::clazz;
jfieldID JavaPatternBuilder::idField;
jmethodID JavaPatternBuilder::constructor;

std::vector<JavaPatternBuilder*> JavaPatternBuilder::builders;

JavaPatternBuilder::JavaPatternBuilder( PatternBuilder * builder, JNIEnv * env ) :
	builder( builder ),
	object( env->NewWeakGlobalRef( env->NewObject( clazz, constructor, (jint)builder->id ) ) ) {
}

JavaPatternBuilder::~JavaPatternBuilder() {
}

JavaPatternBuilder & JavaPatternBuilder::get( JNIEnv * env, jobject obj ) {
	return *builders.at( env->GetIntField( obj, idField ) );
}

JavaPatternBuilder & JavaPatternBuilder::get( JNIEnv * env, PatternBuilder * builder ) {
	if ( builders.size() <= builder->id )
		builders.resize( builder->id + 1 );

	JavaPatternBuilder * result = builders[ builder->id ];

	if ( result ) return *result;

	return *(result = builders[ builder->id ] = new JavaPatternBuilder( builder, env ) );
}

void JavaPatternBuilder::init( JNIEnv * env ) {
	clazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/patterns/PatternBuilder" ) );
	idField = env->GetFieldID( clazz, "id", "I" );
	constructor = env->GetMethodID( clazz, "<init>", "(I)V" );
}

void JavaPatternBuilder::remove( JNIEnv * env, jobject obj ) {
	uint id = env->GetIntField( obj, idField );

	delete builders.at( id );
	builders[ id ] = 0;
}

} } }
