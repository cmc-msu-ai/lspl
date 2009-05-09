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

bool TokenMatcher::matchTransition( const Transition & transition, const Context & context ) const {
	switch ( transition.type ) {
	case Transition::TOKEN: // Слово
		return static_cast<const Token&>( transition ).getToken() == token;
	default:
		return false;
	}
}

TransitionList TokenMatcher::buildTransitions( const text::Node & node, const Context & context ) const {
	TransitionList tokenTransitions;

	for ( uint i = 0; i < node.transitions.size(); ++ i )
		if ( matchTransition( *node.transitions[i], context ) ) {
			tokenTransitions.push_back( node.transitions[i] );
			break;
		}

	return tokenTransitions;
}

void TokenMatcher::dump( std::ostream & out, const std::string & tabs ) const {
	out << "TokenMatcher{ token = \"" << token << "\" }";
}

} } } //namespace lspl::patterns::matchers
