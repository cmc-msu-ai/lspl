#ifndef _LSPL_MATCHERS_STRINGMATCHER_H_
#define _LSPL_MATCHERS_STRINGMATCHER_H_

#include <string>

#include "Forward.h"
#include "Matcher.h"

namespace lspl { namespace patterns { namespace matchers {

class LSPL_EXPORT StringMatcher : public AnnotationMatcher
{
public:
	StringMatcher( const std::string & token ): AnnotationMatcher( TOKEN ), token(token) {};
	virtual ~StringMatcher() {}

	virtual text::TransitionList buildTransitions( const text::Node & node, const Context & context ) const {};
	virtual bool matchTransition( const text::Transition & transition, const Context & context ) const {};
	virtual void dump( std::ostream & out, const std::string & tabs = "" ) const {};

public:
	const std::string token;
};

} } } // namespace lspl::patterns::matchers

#endif /*_LSPL_MATCHERS_STRINGMATCHER_H_*/
