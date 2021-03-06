/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class ru_lspl_text_LoopIteration */

#ifndef _Included_ru_lspl_text_LoopIteration
#define _Included_ru_lspl_text_LoopIteration
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     ru_lspl_text_LoopIteration
 * Method:    internalGetVariants
 * Signature: ()[Lru/lspl/text/LoopIterationVariant;
 */
JNIEXPORT jobjectArray JNICALL Java_ru_lspl_text_LoopIteration_internalGetVariants
  (JNIEnv *, jobject);

/*
 * Class:     ru_lspl_text_LoopIteration
 * Method:    getVariantTransitionCount
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_ru_lspl_text_LoopIteration_getVariantTransitionCount
  (JNIEnv *, jobject, jint);

/*
 * Class:     ru_lspl_text_LoopIteration
 * Method:    getVariantTransition
 * Signature: (II)Lru/lspl/text/Transition;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_text_LoopIteration_getVariantTransition
  (JNIEnv *, jobject, jint, jint);

#ifdef __cplusplus
}
#endif
#endif
