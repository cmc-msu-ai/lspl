#include "JavaText.h"

#include "lspl/java/Utils.h"

#include "lspl/text/Text.h"
#include "lspl/text/readers/PlainTextReader.h"
#include "lspl/text/java/JavaNode.h"

using lspl::text::readers::PlainTextReader;

using lspl::java::in;
using lspl::java::out;

namespace lspl { namespace text { namespace java {

jclass JavaText::clazz;
jfieldID JavaText::idField;
jmethodID JavaText::constructor;

std::vector<JavaText*> JavaText::texts;

JavaText::JavaText( const TextRef & text, JNIEnv * env, jstring content ) :
	text( text ), object( env->NewWeakGlobalRef( env->NewObject( clazz, constructor, (jint)text->id, content ) ) ) {
}

JavaText::JavaText( const TextRef & text, jobject object ) :
	text( text ), object( object ) {
}

JavaText::~JavaText() {
}

JavaText & JavaText::get( JNIEnv * env, jobject obj ) {
	return *texts.at( env->GetIntField( obj, idField ) );
}

JavaText & JavaText::create( JNIEnv * env, jstring content ) {
	return create( env, content, TextConfig() );
}

JavaText & JavaText::create( JNIEnv * env, jstring content, const TextConfig & config ) {
	PlainTextReader reader;
	reader.config = config;
	TextRef text = reader.readFromString( in( env, content ) );

	if ( texts.size() <= text->id )
		texts.resize( text->id + 1 );

	return *(texts[ text->id ] = new JavaText( text, env, content ) );
}

JavaText & JavaText::get( JNIEnv * env, const TextRef & text ) {
	if ( texts.size() <= text->id )
		texts.resize( text->id + 1 );

	JavaText * result = texts[ text->id ];

	if ( result ) return *result;

	return *(texts[ text->id ] = new JavaText( text, env, out( env, text->getContent() ) ) );
}

void JavaText::init( JNIEnv * env ) {
	clazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/text/Text" ) );
	idField = env->GetFieldID( clazz, "id", "I" );
	constructor = env->GetMethodID( clazz, "<init>", "(ILjava/lang/String;)V");
}

void JavaText::remove( JNIEnv * env, jobject obj ) {
	uint id = env->GetIntField( obj, idField );

	delete texts.at( id );
	texts[ id ] = 0;
}

} } }
