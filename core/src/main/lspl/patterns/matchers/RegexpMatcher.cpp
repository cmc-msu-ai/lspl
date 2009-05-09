#include "../../base/BaseInternal.h"

#include "RegexpMatcher.h"

#include "../../text/Node.h"
#include "../../text/markup/Token.h"

#include "../../morphology/Morphology.h"

using lspl::text::Transition;
using lspl::text::TransitionRef;
using lspl::text::TransitionList;
using lspl::text::markup::Token;

using lspl::morphology::Morphology;

namespace lspl { namespace patterns { namespace matchers {

RegexpMatcher::RegexpMatcher( const std::string & token ) : AnnotationMatcher( REGEXP ), exp( Morphology::instance().upcase( token ) ) {}

bool RegexpMatcher::matchTransition( const Transition & transition, const Context & context ) const {
	switch ( transition.type ) {
	case Transition::TOKEN: // Слово
		return boost::regex_match( static_cast<const Token&>( transition ).getToken(), exp );
	default:
		return false;
	}
}

TransitionList RegexpMatcher::buildTransitions( const text::Node & node, const Context & context ) const {
	TransitionList tokenTransitions;

	for ( uint i = 0; i < node.transitions.size(); ++ i )
		if ( matchTransition( *node.transitions[i], context ) ) {
			tokenTransitions.push_back( node.transitions[i] );
			break;
		}

	return tokenTransitions;
}

void RegexpMatcher::dump( std::ostream & out, const std::string & tabs ) const {
	out << "RegexpMatcher{ exp = \"" << exp.str() << "\" }";
}

} } } //namespace lspl::patterns::matchers
