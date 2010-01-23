#ifndef _LSPL_MATCHERS_WORDMATCHER_H_
#define _LSPL_MATCHERS_WORDMATCHER_H_

#include "Forward.h"
#include "Matcher.h"

#include "../../text/attributes/SpeechPart.h"

namespace lspl { namespace patterns { namespace matchers {

/**
 * Сопоставитель слова.
 *
 * Используется для поиска слов с заданной основой и частью речи.
 */
class LSPL_EXPORT WordMatcher : public AnnotationMatcher {
public:

	/**
	 * Конструктор.
	 * @param base основа слова
	 * @param speechPart частьр речи
	 */
	WordMatcher( const std::string & base, text::attributes::SpeechPart speechPart );

	/**
	 * Деструктор
	 */
	virtual ~WordMatcher() {}

	virtual text::TransitionList buildTransitions( const text::Node & node, const Context & context ) const;
	virtual bool matchTransition( const text::Transition & transition, const Context & context ) const;
	virtual void dump( std::ostream & out, const std::string & tabs = "" ) const;
	virtual bool equals( const Matcher & m ) const;

public:

	/**
	 * Основа слова
	 */
	std::string base;

	/**
	 * Часть речи
	 */
	text::attributes::SpeechPart speechPart;
};

} } } // namespace lspl::patterns::matchers

#endif /*_LSPL_MATCHERS_WORDMATCHER_H_*/
