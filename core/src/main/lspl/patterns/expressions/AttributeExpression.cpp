/*
 * AttributeExpression.cpp
 *
 *  Created on: Sep 28, 2008
 *      Author: alno
 */
#include "../../base/BaseInternal.h"
#include "../../text/attributes/AttributeContainer.h"
#include "AttributeExpression.h"
#include "../matchers/Context.h"
#include "../../text/Transition.h"

using namespace lspl::patterns::matchers;
using namespace lspl::text::attributes;

namespace lspl { namespace patterns { namespace expressions {

AttributeExpression::AttributeExpression( const Expression * base, const AttributeKey & attribute ) :
	base( base ), attribute( attribute ) {
}

AttributeExpression::~AttributeExpression() {
}

AttributeValue AttributeExpression::evaluate( const text::Transition & annotation, const Context & ctx ) const {
	AttributeValue baseVal = base->evaluate( annotation, ctx );

	if ( baseVal == AttributeValue::UNDEFINED )
		return AttributeValue::UNDEFINED;

	return baseVal.getContainer().getAttribute( attribute );
}

void AttributeExpression::dump( std::ostream & out, const std::string & tabs ) const {
	out << base << "." << attribute.getAbbrevation();
}

} } }
