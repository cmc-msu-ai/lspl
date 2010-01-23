#include "../../base/BaseInternal.h"

#include "TokenMatcher.h"

#include "../../text/Node.h"
#include "../../text/markup/Token.h"

#include "../../morphology/Morphology.h"

using lspl::text::Transition;
using lspl::text::TransitionRef;
using lspl::text::TransitionList;
using lspl::text::markup::Token;

using lspl::morphology::Morphology;

namespace lspl { namespace patterns { namespace matchers {

TokenMatcher::TokenMatcher( const std::string & token ) : AnnotationMatcher( TOKEN ), token( Morphology::instance().upcase( token ) ) {}

TransitionList TokenMatcher::buildTransitions( const text::Node & node, const Context & context ) const {
	TransitionList transitions;

	for ( uint i = 0, sz = node.getTokenCount(); i < sz; ++ i )
		if ( matchTransition( *node.getTransition( i ), context ) )
			transitions.push_back( node.getTransition( i ) );

	return transitions;
}

bool TokenMatcher::matchTransition( const Transition & transition, const Context & context ) const {
	switch ( transition.type ) {
	case Transition::TOKEN: // Слово
		return static_cast<const Token&>( transition ).getToken() == token;
	default:
		return false;
	}
}

void TokenMatcher::dump( std::ostream & out, const std::string & tabs ) const {
	out << "TokenMatcher{ token = \"" << token << "\" }";
}

} } } //namespace lspl::patterns::matchers
