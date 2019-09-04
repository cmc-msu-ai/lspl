#include "../../base/BaseInternal.h"

#include "OrRestriction.h"

namespace lspl { namespace patterns { namespace restrictions {

OrRestriction::OrRestriction() {
}

OrRestriction::~OrRestriction() {
}

bool OrRestriction::matches( const text::Transition * currentAnnotation, const matchers::Variable currentVar, const matchers::Context & ctx ) const {
	for( boost::ptr_vector<Restriction>::const_iterator it = args.begin(); it != args.end(); ++ it )
		if ( it->matches( currentAnnotation, currentVar, ctx ) )
			return true;

	return false;
}

void OrRestriction::dump( std::ostream & out, const std::string & tabs ) const {
	if ( args.size() <= 1 )
		throw std::logic_error( "Too less arguments" );

	args[0].dump( out );

	for ( uint i = 1; i < args.size(); ++i ) {
		out << " || ";
		args[i].dump( out );
	}
}

bool OrRestriction::equals( const Restriction & r ) const {
	if ( const OrRestriction * ar = dynamic_cast<const OrRestriction *>( &r ) ) {
		if ( ar->args.size() != args.size() ) return false;

		for ( uint i = 0, l = args.size(); i < l; ++ i )
			if ( !ar->args[i].equals( args[i] ) )
				return false;

		return true;
	} else {
		return false;
	}
}

bool OrRestriction::containsVariable( matchers::Variable var ) const {
	for( boost::ptr_vector<Restriction>::const_iterator it = args.begin(); it != args.end(); ++ it )
		if ( it->containsVariable( var ) )
			return true;

	return false;
}

bool OrRestriction::containsVariables() const {
	for( boost::ptr_vector<Restriction>::const_iterator it = args.begin(); it != args.end(); ++ it )
		if ( it->containsVariables() )
			return true;

	return false;
}

bool OrRestriction::containsCurrentAnnotation() const {
	for( boost::ptr_vector<Restriction>::const_iterator it = args.begin(); it != args.end(); ++ it )
		if ( it->containsCurrentAnnotation() )
			return true;

	return false;
}

} } } // namespace lspl::patterns::restrictions
