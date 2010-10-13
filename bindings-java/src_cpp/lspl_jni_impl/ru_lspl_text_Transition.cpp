#include <sstream>

#include "lspl_jni/ru_lspl_text_Transition.h"

#include "lspl/text/Transition.h"
#include "lspl/text/JavaTransition.h"
#include "lspl/text/attributes/java/JavaAttributeValue.h"
#include "lspl/java/Utils.h"

using namespace lspl::java;
using namespace lspl::text;
using namespace lspl::text::attributes::java;

using lspl::text::attributes::AttributeKey;
using lspl::text::attributes::AttributeValue;

/*
 * Class:     ru_lspl_text_Transition
 * Method:    dump
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_text_Transition_dump( JNIEnv * env, jobject obj_transition ) {
	try {
		std::ostringstream dump;
		JavaTransition::get( env, obj_transition )->transition->dump( dump );
		return out( env, dump.str() );
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_text_Transition
 * Method:    finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_ru_lspl_text_Transition_finalize( JNIEnv * env, jobject obj_transition ) {
	try {
		JavaTransition::remove( env, obj_transition );
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
	}
}

/*
 * Class:     ru_lspl_text_Transition
 * Method:    getAttribute
 * Signature: (I)Ljava/lang/Object;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_text_Transition_getAttribute( JNIEnv * env, jobject obj_transition, jint key ) {
	try {
		return JavaAttributeValue::get( env, JavaTransition::get( env, obj_transition )->transition->getAttribute( AttributeKey( key ) ) );
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}
