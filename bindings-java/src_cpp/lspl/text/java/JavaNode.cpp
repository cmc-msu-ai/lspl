#include "JavaNode.h"
#include "JavaText.h"

#include "lspl/text/Node.h"
#include "lspl/text/Text.h"

#include <iostream>

namespace lspl { namespace text { namespace java {

jclass JavaNode::clazz;
jfieldID JavaNode::idField;
jmethodID JavaNode::constructor;

std::vector<JavaNode *> JavaNode::nodes;

JavaNode::JavaNode( NodeRef node, JNIEnv * env ) :
	node( node ),
	object( env->NewWeakGlobalRef( env->NewObject( clazz, constructor, (jint)node->id,
			JavaText::get( env, const_cast<Text*>( &node->text ) ).object, (jint)node->startOffset, (jint)node->endOffset ) ) ) {
}

JavaNode::~JavaNode() {
}

NodeRef JavaNode::get( JNIEnv * env, jobject obj ) {
	return nodes.at( env->GetIntField( obj, idField ) )->node;
}

jobject JavaNode::get( JNIEnv * env, Node * node ) {
	if ( nodes.size() <= node->id )
		nodes.resize( node->id + 1 );

	JavaNode * result = nodes[ node->id ];

	if ( !result ) {
		result = nodes[ node->id ] = new JavaNode( node, env );
	}

	return result->object;
}

void JavaNode::init( JNIEnv * env ) {
	clazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/text/Node" ) );
	idField = env->GetFieldID( clazz, "id", "I" );
	constructor = env->GetMethodID( clazz, "<init>", "(ILru/lspl/text/Text;II)V" );
}

void JavaNode::remove( JNIEnv * env, jobject obj ) {
	uint id = env->GetIntField( obj, idField );

	nodes[ id ] = 0;

	delete nodes.at( id );
}

} } }
