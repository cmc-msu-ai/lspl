#include <sstream>

#include "lspl_jni/ru_lspl_text_Node.h"

#include "lspl/text/Node.h"
#include "lspl/text/java/JavaText.h"
#include "lspl/text/JavaTransition.h"
#include "lspl/java/Utils.h"

using namespace lspl::java;
using namespace lspl::text;
using namespace lspl::text::java;

/*
 * Class:     ru_lspl_text_Node
 * Method:    dump
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_text_Node_dump(JNIEnv * env, jobject obj_node ) {
	std::ostringstream dump;
	JavaText::getNode( env, obj_node ).dump( dump );
	return out( env, dump.str() );
}

/*
 * Class:     ru_lspl_text_Node
 * Method:    getTransition
 * Signature: (I)Lru/lspl/text/Transition;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_text_Node_getTransition(JNIEnv * env, jobject obj_node, jint index) {
	return JavaTransition::get( env, JavaText::getNode( env, obj_node ).getTransition( index ).get() )->object;
}

/*
 * Class:     ru_lspl_text_Node
 * Method:    getTransitionCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_ru_lspl_text_Node_getTransitionCount(JNIEnv * env, jobject obj_node) {
	return JavaText::getNode( env, obj_node ).getTransitionCount();
}
