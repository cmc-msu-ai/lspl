#include "TextDataBuilderConfig.h"

namespace lspl { namespace java {

jclass TextDataBuilderConfig::clazz;
jfieldID TextDataBuilderConfig::analyzePunctuationField;
jfieldID TextDataBuilderConfig::analyzeSpacesField;
jfieldID TextDataBuilderConfig::splitToSentencesField;

void TextDataBuilderConfig::init( JNIEnv * env ) {
	clazz = env->FindClass( "ru/lspl/text/TextConfig" );
	analyzePunctuationField = env->GetFieldID( clazz, "analyzePunctuation", "Z" );
	analyzeSpacesField = env->GetFieldID( clazz, "analyzeSpaces", "Z" );
	splitToSentencesField = env->GetFieldID( clazz, "splitToSentences", "Z" );
}

text::TextConfig TextDataBuilderConfig::fromJava( JNIEnv * env, jobject obj ) {
	text::TextConfig config;
	
	config.analyzePunctuation = env->GetBooleanField( obj, analyzePunctuationField );
	config.analyzeSpaces = env->GetBooleanField( obj, analyzeSpacesField );
	config.splitToSentences = env->GetBooleanField( obj, splitToSentencesField );
	
	return config;
}

} }
