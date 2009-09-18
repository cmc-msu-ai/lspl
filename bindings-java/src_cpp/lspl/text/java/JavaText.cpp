#include "JavaText.h"

#include "lspl/java/Utils.h"

#include "lspl/text/Text.h"
#include "lspl/text/java/JavaNode.h"

namespace lspl { namespace text { namespace java {

jclass JavaText::clazz;
jfieldID JavaText::idField;
jmethodID JavaText::constructor;

std::vector<JavaText*> JavaText::texts;

JavaText::JavaText( const TextRef & text, JNIEnv * env ) :
	text( text ), object( env->NewWeakGlobalRef( env->NewObject( clazz, constructor, (jint)text->id ) ) ) {
}

JavaText::JavaText( const TextRef & text, jobject object ) :
	text( text ), object( object ) {
}

JavaText::~JavaText() {
}

JavaText & JavaText::get( JNIEnv * env, jobject obj ) {
	return *texts.at( env->GetIntField( obj, idField ) );
}

JavaText & JavaText::get( JNIEnv * env, const TextRef & text ) {
	if ( texts.size() <= text->id )
		texts.resize( text->id + 1 );

	JavaText * result = texts[ text->id ];

	if ( result ) return *result;

	return *(texts[ text->id ] = result = new JavaText( text, env ) );
}

void JavaText::init( JNIEnv * env ) {
	clazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/text/Text" ) );
	idField = env->GetFieldID( clazz, "id", "I" );
	constructor = env->GetMethodID( clazz, "<init>", "(I)V");
}

void JavaText::remove( JNIEnv * env, jobject obj ) {
	uint id = env->GetIntField( obj, idField );

	delete texts.at( id );
	texts[ id ] = 0;
}

} } }
