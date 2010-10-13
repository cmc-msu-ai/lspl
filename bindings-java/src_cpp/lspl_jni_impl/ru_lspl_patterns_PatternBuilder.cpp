#include <jni.h>
#include <stdio.h>

#include <sstream>
#include <map>

#include "lspl_jni/ru_lspl_patterns_PatternBuilder.h"

#include "lspl/java/JavaPatternBuilder.h"
#include "lspl/java/JavaPattern.h"
#include "lspl/java/JavaNamespace.h"
#include "lspl/java/Utils.h"

#include "lspl/patterns/PatternBuilder.h"
#include "lspl/patterns/Pattern.h"

#include "lspl/transforms/JavaTransformBuilder.h"

using namespace lspl::java;
using namespace lspl::patterns;
using namespace lspl::patterns::java;

using lspl::Namespace;
using lspl::NamespaceRef;
using lspl::transforms::TransformBuilder;
using lspl::transforms::DummyTransformBuilder;
using lspl::transforms::JavaTransformBuilder;


/*
 * Class:     ru_lspl_patterns_PatternBuilder
 * Method:    create
 * Signature: (Lru/lspl/Namespace;Lru/lspl/transforms/TransformBuilder;)Lru/lspl/patterns/PatternBuilder;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_patterns_PatternBuilder_create(JNIEnv * env, jclass, jobject nsObj, jobject tbObj) {
	try {
		TransformBuilder * tBuilder = tbObj ? static_cast<TransformBuilder *>( new JavaTransformBuilder( env, tbObj ) ) : static_cast<TransformBuilder *>( new DummyTransformBuilder() );
		NamespaceRef ns = JavaNamespace::get( env, nsObj ).ns;

		return JavaPatternBuilder::get( env, new PatternBuilder( ns, tBuilder ) ).object;
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_patterns_PatternBuilder
 * Method:    getNamespace
 * Signature: ()Lru/lspl/Namespace;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_patterns_PatternBuilder_getNamespace(JNIEnv * env, jobject obj) {
	try {
		return JavaNamespace::get( env, JavaPatternBuilder::get( env, obj ).builder->space.get() ).object;
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_PatternBuilder
 * Method:    getDefinedPatternCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_ru_lspl_patterns_PatternBuilder_getDefinedPatternCount(JNIEnv * env, jobject obj) {
	try {
		return JavaPatternBuilder::get( env, obj ).builder->space->getPatternCount();
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_PatternBuilder
 * Method:    getDefinedPattern
 * Signature: (I)Lru/lspl/Pattern;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_patterns_PatternBuilder_getDefinedPattern__I(JNIEnv * env, jobject obj, jint index) {
	try {
		JavaPatternBuilder & data = JavaPatternBuilder::get( env, obj );

		return JavaPattern::get( env, data.builder->space->getPatternByIndex( index ).get() ).object;
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
		return 0;
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}

/*
 * Class:     ru_lspl_PatternBuilder
 * Method:    getDefinedPattern
 * Signature: (Ljava/lang/String;)Lru/lspl/Pattern;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_patterns_PatternBuilder_getDefinedPattern__Ljava_lang_String_2(JNIEnv * env, jobject obj, jstring name) {
	try {
		JavaPatternBuilder & data = JavaPatternBuilder::get( env, obj );

		Pattern * pattern = data.builder->space->getPatternByName( in( env, name ) ).get();

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
 * Class:     ru_lspl_PatternBuilder
 * Method:    build
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_ru_lspl_patterns_PatternBuilder_build(JNIEnv * env, jobject obj, jstring source) {
	try {
		JavaPatternBuilder::get( env, obj ).builder->build( in( env, source ) );
	} catch ( const std::exception & ex ) {
		env->ThrowNew( env->FindClass("ru/lspl/patterns/PatternBuildingException"), ex.what() );
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
	}
}

/*
 * Class:     ru_lspl_PatternBuilder
 * Method:    finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_ru_lspl_patterns_PatternBuilder_finalize(JNIEnv * env, jobject obj) {
	try {
		JavaPatternBuilder::remove( env, obj );
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
	}
}
