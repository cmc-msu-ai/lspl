#include "lspl_jni/ru_lspl_text_Loop.h"

#include "lspl/text/Loop.h"
#include "lspl/text/JavaLoop.h"
#include "lspl/java/Utils.h"

using namespace lspl::java;
using namespace lspl::text;

/*
 * Class:     ru_lspl_text_Loop
 * Method:    getTransitionCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_ru_lspl_text_Loop_getTransitionCount(JNIEnv * env, jobject obj) {
	return JavaLoop::get(env,obj)->transition.cast<Loop>()->transitions.size();
}

/*
 * Class:     ru_lspl_text_Loop
 * Method:    getTransition
 * Signature: (I)Lru/lspl/text/Transition;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_text_Loop_getTransition(JNIEnv * env, jobject obj, jint index) {
	return JavaTransition::get( env, JavaLoop::get(env,obj)->transition.cast<Loop>()->transitions.at( index ).get() )->object;
}
