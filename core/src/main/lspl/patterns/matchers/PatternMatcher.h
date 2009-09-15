#ifndef _LSPL_MATCHERS_PATTERNMATCHER_H_
#define _LSPL_MATCHERS_PATTERNMATCHER_H_

#include "Forward.h"
#include "Matcher.h"

#include "../Forward.h"

namespace lspl { namespace patterns { namespace matchers {

/**
 * Сопоставитель шаблона.
 *
 * Сопоставитель шаблона используется для поиска в в тексте уже найденных ранее шаблонов заданного типа.
 */
class LSPL_EXPORT PatternMatcher : public AnnotationMatcher
{
public:
	PatternMatcher( const Pattern & pattern );
	virtual ~PatternMatcher();

	virtual text::TransitionList buildTransitions( const text::Node & node, const Context & context ) const;
	virtual bool matchTransition(const text::Transition & transition, const Context & context) const;
	virtual void dump( std::ostream & out, const std::string & tabs = "" ) const;
	virtual bool equals( const Matcher & m ) const;

	void buildTransitions( const text::Node & node, const patterns::Pattern & pattern, const patterns::Alternative & alt, const Context & context, text::TransitionList & results ) const;
	void buildTransitions( const text::Transition & transition, const patterns::Pattern & pattern, const patterns::Alternative & alt, const Context & context, text::TransitionList & results ) const;

public:
	const Pattern & pattern;
};

} } } // namespace lspl::patterns::matchers

#endif /*_LSPL_MATCHERS_PATTERNMATCHER_H_*/
