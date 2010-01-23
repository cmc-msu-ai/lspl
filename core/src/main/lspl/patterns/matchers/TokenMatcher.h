#ifndef _LSPL_MATCHERS_TOKENMATCHER_H_
#define _LSPL_MATCHERS_TOKENMATCHER_H_

#include <string>

#include "Forward.h"
#include "Matcher.h"

namespace lspl { namespace patterns { namespace matchers {

class LSPL_EXPORT TokenMatcher : public AnnotationMatcher
{
public:
	TokenMatcher( const std::string & token );
	virtual ~TokenMatcher() {}

	virtual text::TransitionList buildTransitions( const text::Node & node, const Context & context ) const;
	virtual bool matchTransition( const text::Transition & transition, const Context & context ) const;
	virtual void dump( std::ostream & out, const std::string & tabs = "" ) const;

public:
	const std::string token;
};

} } } // namespace lspl::patterns::matchers

#endif /*_LSPL_MATCHERS_TOKENMATCHER_H_*/
