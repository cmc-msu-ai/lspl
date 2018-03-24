#ifndef _LSPL_MATCHERS_REGEXMATCHER_H_
#define _LSPL_MATCHERS_REGEXMATCHER_H_

#include "Forward.h"
#include "Matcher.h"

#ifdef WIN32
#define PCRE_STATIC
#include <pcrecpp.h>
#undef PCRE_STATIC
#else
#include <pcrecpp.h>
#endif

namespace lspl { namespace patterns { namespace matchers {

class LSPL_EXPORT RegexpMatcher : public AnnotationMatcher
{
public:
	RegexpMatcher( const std::string & token );
	virtual ~RegexpMatcher() {}

	virtual text::TransitionList buildTransitions( const text::Node & node, const Context & context ) const;
	virtual bool matchTransition( const text::Transition & transition, const Context & context ) const;
	virtual void dump( std::ostream & out, const std::string & tabs = "" ) const;

public:
	const pcrecpp::RE exp;
};

} } } // namespace lspl::patterns::matchers

#endif /*_LSPL_MATCHERS_REGEXMATCHER_H_*/
