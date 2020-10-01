#ifndef _LSPL_MATCHERS_WORDMATCHER_H_
#define _LSPL_MATCHERS_WORDMATCHER_H_

#include <memory>

#include "Forward.h"
#include "Matcher.h"
#include "BaseComparator.h"

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
	 * @param speechPart часть речи
	 */
	WordMatcher( text::attributes::SpeechPart speechPart, BaseComparator* baseComparator = nullptr );

	/**
	 * Деструктор
	 */
	virtual ~WordMatcher() {}

	virtual text::TransitionList buildTransitions( const text::Node & node, const Context & context ) const;
	virtual bool matchTransition( const text::Transition & transition, const Context & context ) const;
	virtual void dump( std::ostream & out, const std::string & tabs = "" ) const;
	virtual bool equals( const Matcher & m ) const;
	virtual void setBaseComparator(BaseComparator *);

public:
	/**
	 * Часть речи
	 */
	text::attributes::SpeechPart speechPart;

	/**
	 * Ограничение на основу/лемму
	 */
	std::unique_ptr<BaseComparator> baseComparator;
};

} } } // namespace lspl::patterns::matchers

#endif /*_LSPL_MATCHERS_WORDMATCHER_H_*/
