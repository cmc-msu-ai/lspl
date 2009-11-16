#include "lspl_jni/ru_lspl_text_LoopIteration.h"

#include "lspl/text/Loop.h"
#include "lspl/text/JavaLoopIteration.h"
#include "lspl/java/Utils.h"

using namespace lspl::java;
using namespace lspl::text;

/*
 * Class:     ru_lspl_text_LoopIteration
 * Method:    getVariantCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_ru_lspl_text_LoopIteration_getVariantCount(JNIEnv * env, jobject obj) {
	return JavaTransition::get(env,obj)->transition.cast<LoopIteration>()->getVariantCount();
}

/*
 * Class:     ru_lspl_text_LoopIteration
 * Method:    getVariant
 * Signature: (I)Lru/lspl/text/LoopIterationVariant;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_text_LoopIteration_getVariant(JNIEnv * env, jobject obj, jint vindex) {
	return JavaLoopIteration::get(env,obj)->getVariant( env, vindex );
}

/*
 * Class:     ru_lspl_text_LoopIteration
 * Method:    getVariantTransitionCount
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_ru_lspl_text_LoopIteration_getVariantTransitionCount(JNIEnv * env, jobject obj, jint vindex) {
	return JavaTransition::get(env,obj)->transition.cast<LoopIteration>()->getVariant( vindex ).size();
}

/*
 * Class:     ru_lspl_text_LoopIteration
 * Method:    getVariantTransition
 * Signature: (II)Lru/lspl/text/Transition;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_text_LoopIteration_getVariantTransition(JNIEnv * env, jobject obj, jint vindex, jint tindex) {
	return JavaTransition::get( env, JavaTransition::get(env,obj)->transition.cast<LoopIteration>()->getVariant( vindex ).at( tindex ).get() )->object;
}

/*
 * Class:     ru_lspl_text_LoopIteration
 * Method:    finalizeVariant
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_ru_lspl_text_LoopIteration_finalizeVariant(JNIEnv * env, jobject obj, jint vindex) {
	JavaLoopIteration * m = JavaLoopIteration::get(env,obj);

	if ( m ) m->freeVariant( vindex );
}
