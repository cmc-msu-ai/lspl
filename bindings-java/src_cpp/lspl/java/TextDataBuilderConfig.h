#ifndef TEXTDATABUILDERCONFIG_H_
#define TEXTDATABUILDERCONFIG_H_

#include <jni.h>

#include "lspl/base/Base.h"
#include "lspl/text/TextConfig.h"

namespace lspl { namespace java {

class TextDataBuilderConfig
{
public:
	static void init( JNIEnv * env );
	static text::TextConfig fromJava( JNIEnv * env, jobject obj );

private:
	static jclass 	clazz;
	static jfieldID analyzePunctuationField;
	static jfieldID analyzeSpacesField;
	static jfieldID splitToSentencesField;
};

} }

#endif /*TEXTDATABUILDERCONFIG_H_*/
