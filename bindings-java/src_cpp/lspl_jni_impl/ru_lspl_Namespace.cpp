#include <jni.h>
#include <stdio.h>

#include <sstream>
#include <map>

#include <lspl/patterns/Pattern.h>

#include "lspl_jni/ru_lspl_Namespace.h"

#include "lspl/java/JavaNamespace.h"
#include "lspl/java/JavaPattern.h"
#include "lspl/java/Utils.h"

using namespace lspl::java;
using namespace lspl::patterns;
using namespace lspl::patterns::java;

using lspl::Namespace;
using lspl::NamespaceRef;
using lspl::NamespaceList;

/*
 * Class:     ru_lspl_Namespace
 * Method:    create
 * Signature: ([Lru/lspl/Namespace;)Lru/lspl/Namespace;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_Namespace_create(JNIEnv * env, jclass cls, jobjectArray parentArray) {
	try {
		NamespaceList parents;

		for ( int i = 0, parentCount = env->GetArrayLength( parentArray ); i < parentCount; ++ i ) {
			jobject parentObj = env->GetObjectArrayElement( parentArray, i );

			parents.push_back( JavaNamespace::get( env, parentObj ).ns );
		}

		return JavaNamespace::get( env, new Namespace( parents ) ).object;
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_Namespace
 * Method:    getDefinedPatternCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_ru_lspl_Namespace_getDefinedPatternCount(JNIEnv * env, jobject obj) {
	try {
		return JavaNamespace::get( env, obj ).ns->getPatternCount();
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_Namespace
 * Method:    getDefinedPattern
 * Signature: (I)Lru/lspl/patterns/Pattern;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_Namespace_getDefinedPattern__I(JNIEnv * env, jobject obj, jint index) {
	try {
		JavaNamespace & data = JavaNamespace::get( env, obj );

		return JavaPattern::get( env, data.ns->getPatternByIndex( index ).get() ).object;
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_Namespace
 * Method:    getDefinedPattern
 * Signature: (Ljava/lang/String;)Lru/lspl/patterns/Pattern;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_Namespace_getDefinedPattern__Ljava_lang_String_2(JNIEnv * env, jobject obj, jstring name) {
	try {
		JavaNamespace & data = JavaNamespace::get( env, obj );

		Pattern * pattern = data.ns->getPatternByName( in( env, name ) ).get();

		if ( !pattern )
			return 0;

		return JavaPattern::get( env, pattern ).object;
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_Namespace
 * Method:    finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_ru_lspl_Namespace_finalize(JNIEnv * env, jobject obj) {
	try {
		JavaNamespace::remove( env, obj );
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
	}
}
