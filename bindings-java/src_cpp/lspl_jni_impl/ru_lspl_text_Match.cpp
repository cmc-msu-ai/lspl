#include "lspl_jni/ru_lspl_text_Match.h"

#include "lspl/text/Match.h"
#include "lspl/text/JavaMatch.h"
#include "lspl/java/JavaAlternative.h"
#include "lspl/java/Utils.h"

using namespace lspl::java;
using namespace lspl::patterns::java;
using namespace lspl::text;

/*
 * Class:     ru_lspl_text_Match
 * Method:    getVariantCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_ru_lspl_text_Match_getVariantCount(JNIEnv * env, jobject obj) {
	return JavaTransition::get(env,obj)->transition.cast<Match>()->getVariants().size();
}

/*
 * Class:     ru_lspl_text_Match
 * Method:    getVariant
 * Signature: (I)Lru/lspl/text/MatchVariant;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_text_Match_getVariant(JNIEnv * env, jobject obj, jint index) {
	return JavaMatch::get(env,obj)->getVariant( env, index );
}

/*
 * Class:     ru_lspl_text_Match
 * Method:    getVariantTransformResult
 * Signature: (I)Ljava/lang/Object;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_text_Match_getVariantTransformResult(JNIEnv * env, jobject obj, jint index) {
	try {
		return JavaMatch::get(env,obj)->transition.cast<Match>()->getVariant( index )->getTransformResult<jobject>();
	} catch ( lspl::base::Exception & e ) {
		return 0;
	}
}

/*
 * Class:     ru_lspl_text_Match
 * Method:    getVariantTransitionCount
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_ru_lspl_text_Match_getVariantTransitionCount(JNIEnv * env, jobject obj, jint vindex) {
	return JavaMatch::get(env,obj)->transition.cast<Match>()->getVariant( vindex )->size();
}

/*
 * Class:     ru_lspl_text_Match
 * Method:    getVariantAlternative
 * Signature: (I)Lru/lspl/patterns/Alternative;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_text_Match_getVariantAlternative(JNIEnv * env, jobject obj, jint vindex) {
    MatchRef match = JavaMatch::get(env,obj)->transition.cast<Match>();

    return JavaAlternative::get( env, &match->getPattern(), &match->getVariant( vindex )->alternative ).object;
}

/*
 * Class:     ru_lspl_text_Match
 * Method:    getVariantTransition
 * Signature: (II)Lru/lspl/text/Transition;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_text_Match_getVariantTransition(JNIEnv * env, jobject obj, jint vindex, jint tindex) {
	return JavaTransition::get( env, JavaMatch::get(env,obj)->transition.cast<Match>()->getVariant( vindex )->at( tindex ).get() )->object;
}

/*
 * Class:     ru_lspl_text_Match
 * Method:    finalizeVariant
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_ru_lspl_text_Match_finalizeVariant(JNIEnv * env, jobject obj, jint vindex) {
	JavaMatch * m = JavaMatch::get(env,obj);

	if ( m ) m->freeVariant( vindex );
}
