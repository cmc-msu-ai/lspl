#include "lspl_jni/ru_lspl_text_LoopIteration.h"

#include "lspl/text/Loop.h"
#include "lspl/text/JavaLoopIteration.h"
#include "lspl/java/Utils.h"

using namespace lspl::java;
using namespace lspl::text;

/*
 * Class:     ru_lspl_text_LoopIteration
 * Method:    internalGetVariants
 * Signature: ()[Lru/lspl/text/LoopIterationVariant;
 */
JNIEXPORT jobjectArray JNICALL Java_ru_lspl_text_LoopIteration_internalGetVariants(JNIEnv * env, jobject obj) {
	const LoopIteration & iteration = *JavaTransition::get( env, obj )->transition.cast<LoopIteration>();

	jobjectArray result = env->NewObjectArray( iteration.getVariantCount(), JavaLoopIteration::variantClazz, 0 );

	if ( result == 0 )
		return 0;

	for ( int i = 0, sz = iteration.getVariantCount(); i < sz; ++ i ) {
		const LoopIterationVariant & variant = iteration.getVariant( i );

		jobject varObj = env->NewObject( JavaLoopIteration::variantClazz, JavaLoopIteration::variantConstructor, obj, i );

		env->SetObjectArrayElement( result, i, varObj );
	}

	return result;
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
