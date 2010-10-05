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
jfieldID JavaText::nodeIndexField;
jfieldID JavaText::nodeTextField;
jmethodID JavaText::nodeConstructor;

jclass JavaText::transitionClazz;
jfieldID JavaText::transitionNodeIndexField;
jfieldID JavaText::transitionIndexField;
jfieldID JavaText::transitionTextField;

jmethodID JavaText::transitionConstructor;

std::vector<JavaText*> JavaText::texts;

JavaText::JavaText( const TextRef & text, JNIEnv * env, jstring content ) :
	text( text ), object( env->NewWeakGlobalRef( env->NewObject( clazz, constructor, (jint)text->id, content ) ) ) {

	if ( texts.size() <= text->id )
		texts.resize( text->id + 1 );
		
	texts[ text->id ] = this;

	env->CallVoidMethod( object, initializer, createTextNodeArray( env, object ) );
}

JavaText::JavaText( const TextRef & text, jobject object ) :
	text( text ), object( object ) {
}

JavaText::~JavaText() {
}

jobjectArray JavaText::createTextNodeArray( JNIEnv * env, jobject obj ) {
	const TextRef & text = get( env, obj ).text;

	jobjectArray result = env->NewObjectArray( text->getNodes().size(), nodeClazz, 0 );

	if ( result == 0 )
		return 0;

	for ( uint i = 0, sz = text->getNodes().size(); i < sz; ++ i ) {
		const Node & node = *text->getNodes().at( i );
		jobject nodeObj = env->NewObject( nodeClazz, nodeConstructor, (jint)node.index, obj, (jint)node.startOffset, (jint)node.endOffset );

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
	nodeIndexField = env->GetFieldID( nodeClazz, "id", "I" );
	nodeTextField = env->GetFieldID( nodeClazz, "text", "Lru/lspl/text/Text;" );
	nodeConstructor = env->GetMethodID( nodeClazz, "<init>", "(ILru/lspl/text/Text;II)V" );

	transitionClazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/text/Transition" ) );
	transitionIndexField = env->GetFieldID( transitionClazz, "id", "I" );
	transitionNodeIndexField = env->GetFieldID( transitionClazz, "nodeIndex", "I" );
	transitionIndexField = env->GetFieldID( transitionClazz, "id", "I" );
	transitionConstructor = env->GetMethodID( transitionClazz, "<init>", "(ILru/lspl/text/Text;II)V" );
}

void JavaText::remove( JNIEnv * env, jobject obj ) {
	uint id = env->GetIntField( obj, idField );

	delete texts.at( id );
	texts[ id ] = 0;
}

Node & JavaText::getNode( JNIEnv * env, jobject obj ) {
	return *get( env, env->GetObjectField( obj, nodeTextField ) ).text->getNodes().at( env->GetIntField( obj, nodeIndexField ) );
}

Transition & JavaText::getTransition( JNIEnv * env, jobject obj ) {
	return *get( env, env->GetObjectField( obj, transitionTextField ) ).text->getNodes().at( env->GetIntField( obj, transitionNodeIndexField ) )->getTransitions().at( env->GetIntField( obj, transitionIndexField ) );
}

} } }
