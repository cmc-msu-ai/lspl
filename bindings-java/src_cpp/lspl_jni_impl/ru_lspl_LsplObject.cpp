#include "lspl_jni/ru_lspl_LsplObject.h"


#include "lspl/java/Utils.h"
#include "lspl/java/TextDataBuilderConfig.h"
#include "lspl/java/JavaPattern.h"
#include "lspl/java/JavaPatternBuilder.h"

#include "lspl/java/JavaAlternative.h"

#include "lspl/text/java/JavaText.h"
#include "lspl/text/java/JavaNode.h"
#include "lspl/text/java/JavaTransition.h"
#include "lspl/text/attributes/java/JavaAttributeValue.h"

using namespace lspl::java;
using namespace lspl::patterns::java;
using namespace lspl::text::java;
using namespace lspl::text::attributes::java;

/*
 * Class:     ru_lspl_LsplObject
 * Method:    initStatic
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_ru_lspl_LsplObject_initStatic(JNIEnv * env, jclass cls) {
	JavaText::init( env );
	JavaNode::init( env );
	JavaTransition::init( env );
	JavaAttributeValue::init( env );
	
	JavaAlternative::init( env );
	JavaPattern::init( env );
	JavaPatternBuilder::init( env );
	
	TextDataBuilderConfig::init( env );	
}
