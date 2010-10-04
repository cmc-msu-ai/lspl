#include "JavaText.h"

#include "lspl/java/Utils.h"

#include "lspl/text/Text.h"
#include "lspl/text/readers/PlainTextReader.h"

using lspl::text::readers::PlainTextReader;

using lspl::java::in;
using lspl::java::out;

namespace lspl { namespace text { namespace java {

jclass JavaText::clazz;
jfieldID JavaText::idField;
jmethodID JavaText::constructor;
jmethodID JavaText::initializer;

jclass JavaText::nodeClazz;
jfieldID JavaText::nodeIdField;
jfieldID JavaText::nodeTextField;
jmethodID JavaText::nodeConstructor;

std::vector<JavaText*> JavaText::texts;

JavaText::JavaText( const TextRef & text, JNIEnv * env, jstring content ) :
	text( text ), object( env->NewWeakGlobalRef( env->NewObject( clazz, constructor, (jint)text->id, content ) ) ) {

	if ( texts.size() <= text->id )
		texts.resize( text->id + 1 );
		
	texts[ text->id ] = this;

	env->CallVoidMethod( object, initializer, createNodeArray( env ) );
}

JavaText::JavaText( const TextRef & text, jobject object ) :
	text( text ), object( object ) {
}

JavaText::~JavaText() {
}

jobjectArray JavaText::createNodeArray( JNIEnv * env ) {
	jobjectArray result = env->NewObjectArray( text->getNodes().size(), nodeClazz, 0 );
	
	if ( result == 0 )
		return 0;
		
	for ( uint i = 0, sz = text->getNodes().size(); i < sz; ++ i ) {
		const Node & node = *text->getNodes().at( i );
		jobject nodeObj = env->NewObject( nodeClazz, nodeConstructor, (jint)node.index, object, (jint)node.startOffset, (jint)node.endOffset );

		env->SetObjectArrayElement( result, i, nodeObj );
	}
	
	return result;
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

	return *new JavaText( text, env, content );
}

JavaText & JavaText::get( JNIEnv * env, const Text & text ) {
	if ( texts.size() > text.id && texts[ text.id ] )
		return *texts[ text.id ];
	else
		return *new JavaText( const_cast<Text*>( &text ), env, out( env, text.getContent() ) );
}

void JavaText::init( JNIEnv * env ) {
	clazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/text/Text" ) );
	idField = env->GetFieldID( clazz, "id", "I" );
	constructor = env->GetMethodID( clazz, "<init>", "(ILjava/lang/String;)V");
	initializer = env->GetMethodID( clazz, "initialize", "([Lru/lspl/text/Node;)V");

	nodeClazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/text/Node" ) );
	nodeIdField = env->GetFieldID( nodeClazz, "id", "I" );
	nodeTextField = env->GetFieldID( nodeClazz, "text", "Lru/lspl/text/Text;" );
	nodeConstructor = env->GetMethodID( nodeClazz, "<init>", "(ILru/lspl/text/Text;II)V" );
}

void JavaText::remove( JNIEnv * env, jobject obj ) {
	uint id = env->GetIntField( obj, idField );

	delete texts.at( id );
	texts[ id ] = 0;
}

Node & JavaText::getNode( JNIEnv * env, jobject obj ) {
	return *get( env, env->GetObjectField( obj, nodeTextField ) ).text->getNodes().at( env->GetIntField( obj, nodeIdField ) );
}

} } }
