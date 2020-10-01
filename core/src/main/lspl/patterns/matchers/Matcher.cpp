#include "../../base/BaseInternal.h"

#include "Matcher.h"

#include "../../text/Transition.h"
#include "../../text/Node.h"

using lspl::text::Transition;
using lspl::text::TransitionList;
using lspl::text::TransitionRef;

using lspl::text::attributes::AttributeKey;
using lspl::text::attributes::AttributeType;
using lspl::text::attributes::AttributeValue;

namespace lspl { namespace patterns { namespace matchers {

uint Matcher::aliveObjectsCount = 0;

Matcher::Matcher( Type type ) :
	type( type ) {
	++ aliveObjectsCount;
}

Matcher::~Matcher() {
	-- aliveObjectsCount;
}

bool Matcher::equals( const Matcher & m ) const {
	if ( m.type != type ) return false; // Разные типы
	if ( m.variable != variable ) return false; // Разные переменные
	if ( m.restrictions.size() != restrictions.size() ) return false; // Разное количество ограничений

	for ( uint i = 0, l = restrictions.size(); i < l; ++ i )
		if ( !m.restrictions[i].equals( restrictions[i] ) )
			return false;

	return true;
}

bool Matcher::matchRestrictions( const Transition & annotation, const Context & ctx ) const {
	for ( uint i = 0; i < restrictions.size(); ++ i ) // Проверяем соответствие ограничениям
		if ( !restrictions[ i ].matches( &annotation, variable, ctx ) )
			return false;

	return true;
}

void Matcher::dumpRestrictions( std::ostream & out, const std::string & tabs ) const {
	out << "< ";

	for ( uint i = 0; i < restrictions.size(); ++ i ) {
		if ( i != 0 )
			out << ", ";

		restrictions[i].dump( out, tabs + "\t" );
	}

	out << " >";
}

bool Matcher::containsVariable(const Variable &v) const {
	return variable == v;
}

TransitionList AnnotationMatcher::buildTransitions( const text::Node & node, const Context & context ) const {
	TransitionList transitions;

	for ( uint i = 0, sz = node.getTransitions().size(); i < sz; ++ i )
		if ( matchTransition( *node.getTransition( i ), context ) )
			transitions.push_back( node.getTransition( i ) );

	return transitions;
}

} } } // namespace lspl::patterns::matchers
