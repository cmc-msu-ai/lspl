#include "lspl_jni/ru_lspl_LsplObject.h"

#include "lspl/java/Utils.h"
#include "lspl/java/JavaNamespace.h"
#include "lspl/java/TextDataBuilderConfig.h"
#include "lspl/java/JavaPattern.h"
#include "lspl/java/JavaPatternBuilder.h"

#include "lspl/java/JavaAlternative.h"

#include "lspl/text/java/JavaText.h"
#include "lspl/text/JavaTransition.h"
#include "lspl/text/JavaLoop.h"
#include "lspl/text/JavaLoopIteration.h"
#include "lspl/text/JavaMatch.h"
#include "lspl/text/JavaToken.h"
#include "lspl/text/JavaWord.h"
#include "lspl/text/attributes/java/JavaAttributeValue.h"

#include "lspl/transforms/JavaTransform.h"
#include "lspl/transforms/JavaTransformBuilder.h"

#include "lspl/patterns/Pattern.h"
#include "lspl/patterns/matchers/Matcher.h"
#include "lspl/patterns/restrictions/Restriction.h"
#include "lspl/text/Text.h"
#include "lspl/text/Node.h"
#include "lspl/text/Transition.h"

#include <stdlib.h>

#include <sstream>

#ifdef WIN32
	#include <windows.h>
	#include <winuser.h>
#endif

using namespace lspl::java;
using namespace lspl::patterns;
using namespace lspl::patterns::java;
using namespace lspl::patterns::matchers;
using namespace lspl::patterns::restrictions;
using namespace lspl::text;
using namespace lspl::text::java;
using namespace lspl::text::attributes::java;
using namespace lspl::transforms;

/*
 * Class:     ru_lspl_LsplObject
 * Method:    initStatic
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_ru_lspl_LsplObject_initStatic(JNIEnv * env, jclass cls) {
	setupVM( env );

	try {
		JavaNamespace::init( env );

		JavaText::init( env );
		JavaTransition::init( env );

		JavaLoop::init( env );
		JavaLoopIteration::init( env );
		JavaMatch::init( env );
		JavaToken::init( env );
		JavaWord::init( env );
	
		JavaAttributeValue::init( env );

		JavaAlternative::init( env );
		JavaPattern::init( env );
		JavaPatternBuilder::init( env );

		JavaTransform::init( env );
		JavaTransformBuilder::init( env );
	
		TextDataBuilderConfig::init( env );
	} catch ( const std::exception & ex ) {
		throwRuntimeException( env, ex.what() );
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
	}
}

/*
 * Class:     ru_lspl_LsplObject
 * Method:    setRml
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_ru_lspl_LsplObject_setRml(JNIEnv * env, jclass cls, jstring rml) {
	try {
		std::string s = in( env, rml );

		#ifdef WIN32
				SetEnvironmentVariable( "RML", s.c_str() );
		#else
				setenv( "RML", s.c_str(), 1 );
		#endif
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
	}
}

/*
 * Class:     ru_lspl_LsplObject
 * Method:    dumpMemoryStats
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_LsplObject_dumpMemoryStats(JNIEnv * env, jclass cls) {
	try {
		std::ostringstream output;

		output << "Patterns: " << Pattern::aliveObjectsCount << std::endl;
		output << "Matchers: " << Matcher::aliveObjectsCount << std::endl;
		output << "Restrictions: " << Restriction::aliveObjectsCount << std::endl;

		output << "Texts: " << Text::aliveObjectsCount << std::endl;
		output << "Nodes: " << Node::aliveObjectsCount << std::endl;
		output << "Transitions: " << Transition::aliveObjectsCount << std::endl;

		output << "JavaTransition Vector: " << JavaTransition::transitions.size() << std::endl;

		return out( env, output.str() );
	} catch ( ... ) {
		throwRuntimeException( env, "Unknown error" );
		return 0;
	}
}
