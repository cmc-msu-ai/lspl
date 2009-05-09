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

AttributeValue ConcatenationExpression::evaluate( const text::Transition & annotation, const Context & ctx ) const {
	std::ostringstream out;

	if ( !args.empty() ) {
		out << args[0].evaluate( annotation, ctx ).getString();

		for ( uint i = 1; i < args.size(); ++ i )
			out << " " << args[i].evaluate( annotation, ctx ).getString();
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

} } }
