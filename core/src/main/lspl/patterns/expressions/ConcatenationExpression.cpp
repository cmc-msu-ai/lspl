/*
 * ConcatentaionExpression.cpp
 *
 *  Created on: Sep 30, 2008
 *      Author: alno
 */
#include "../../base/BaseInternal.h"
#include "../../text/attributes/AttributeContainer.h"
#include "ConcatenationExpression.h"

#include <sstream>

using namespace lspl::patterns::matchers;
using namespace lspl::text::attributes;

namespace lspl { namespace patterns { namespace expressions {

ConcatenationExpression::~ConcatenationExpression() {
}

AttributeValue ConcatenationExpression::evaluate( const text::Transition * currentAnnotation, const matchers::Variable currentVar, const Context & ctx ) const {
	std::ostringstream out;

	if ( !args.empty() ) {
		out << args[0].evaluate( currentAnnotation, currentVar, ctx ).getString();

		for ( uint i = 1; i < args.size(); ++ i )
			out << " " << args[i].evaluate( currentAnnotation, currentVar, ctx ).getString();
	}

	return AttributeValue( out.str() );
}

void ConcatenationExpression::dump( std::ostream & out, const std::string & tabs ) const {
	if ( args.empty() )
		return;

	args[0].dump( out, tabs );

	for ( uint i = 1; i < args.size(); ++ i ) {
		out << " + ";
		args[i].dump( out, tabs );
	}
}

bool ConcatenationExpression::equals( const Expression & e ) const {
	if ( const ConcatenationExpression * exp = dynamic_cast<const ConcatenationExpression *>( &e ) ) {
		if ( exp->args.size() != args.size() ) return false;

		for ( uint i = 0, l = args.size(); i < l; ++ i )
			if ( !exp->args[i].equals( args[i] ) )
				return false;

		return true;
	} else {
		return false;
	}
}

bool ConcatenationExpression::containsVariable( matchers::Variable var ) const {
	foreach( const Expression & exp, args )
		if ( exp.containsVariable( var ) )
			return true;

	return false;
}

} } }
