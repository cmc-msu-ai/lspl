#include "JavaWord.h"

#include "java/JavaNode.h"
#include "java/JavaText.h"

#include "lspl/java/Utils.h"
#include "lspl/java/JavaPattern.h"

#include "lspl/patterns/Pattern.h"

#include "lspl/text/Text.h"
#include "lspl/text/Node.h"
#include "lspl/text/Transition.h"
#include "lspl/text/markup/Word.h"

#include <iostream>

using lspl::java::out;

using namespace lspl::text::markup;
using namespace lspl::text::java;

namespace lspl { namespace text {

jclass JavaWord::clazz;
jmethodID JavaWord::constructor;

JavaWord::JavaWord( Transition * t, JNIEnv * env ) {
	Word * word = dynamic_cast<Word*>( t );

	transition = t;
	object = env->NewWeakGlobalRef( env->NewObject(
			clazz, constructor, (jint)transition->id,
			JavaText::get( env, const_cast<Text*>( &transition->start.text ) ).object, // Текст
			JavaNode::get( env, const_cast<Node*>( &transition->start ) ), // Начальный узел
			JavaNode::get( env, const_cast<Node*>( &transition->end ) ), // Конечный узел
			out( env, word->getBase() ), // Начальная форма слова
			JavaTransition::get( env, word->getTokenRef().get() )->object, // Ссылка на лексему
			word->getSpeechPart().id ) ); // Часть речи
}

JavaWord::~JavaWord() {
}

void JavaWord::init( JNIEnv * env ) {
	clazz = (jclass) env->NewGlobalRef( (jobject)env->FindClass( "ru/lspl/text/Word" ) );
	constructor = env->GetMethodID( clazz, "<init>", "(ILru/lspl/text/Text;Lru/lspl/text/Node;Lru/lspl/text/Node;Ljava/lang/String;Lru/lspl/text/Token;I)V" );
}

} }
