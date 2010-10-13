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
 * Method:    internalGetVariants
 * Signature: ()[Lru/lspl/text/MatchVariant;
 */
JNIEXPORT jobjectArray JNICALL Java_ru_lspl_text_Match_internalGetVariants(JNIEnv * env, jobject obj) {
	try {
		const Match & match = *JavaTransition::get(env,obj)->transition.cast<Match>();

		jobjectArray result = env->NewObjectArray( match.getVariantCount(), JavaMatch::variantClazz, 0 );

		if ( result == 0 )
			return 0;

		for ( int i = 0, sz = match.getVariantCount(); i < sz; ++ i ) {
			const MatchVariant & variant = *match.getVariant( i );

			jobject varObj = env->NewObject( JavaMatch::variantClazz, JavaMatch::variantConstructor, obj, i );

			env->SetObjectArrayElement( result, i, varObj );
		}

		return result;
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
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
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_text_Match
 * Method:    getVariantTransitionCount
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_ru_lspl_text_Match_getVariantTransitionCount(JNIEnv * env, jobject obj, jint vindex) {
	try {
		return JavaMatch::get(env,obj)->transition.cast<Match>()->getVariant( vindex )->size();
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_text_Match
 * Method:    getVariantAlternative
 * Signature: (I)Lru/lspl/patterns/Alternative;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_text_Match_getVariantAlternative(JNIEnv * env, jobject obj, jint vindex) {
	try {
		MatchRef match = JavaMatch::get(env,obj)->transition.cast<Match>();

		return JavaAlternative::get( env, &match->getPattern(), &match->getVariant( vindex )->alternative ).object;
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_text_Match
 * Method:    getVariantTransition
 * Signature: (II)Lru/lspl/text/Transition;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_text_Match_getVariantTransition(JNIEnv * env, jobject obj, jint vindex, jint tindex) {
	try {
		return JavaTransition::get( env, JavaMatch::get(env,obj)->transition.cast<Match>()->getVariant( vindex )->at( tindex ).get() )->object;
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

