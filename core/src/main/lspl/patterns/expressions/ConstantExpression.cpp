/*
 * ConstantExpression.cpp
 *
 *  Created on: Sep 28, 2008
 *      Author: alno
 */
#include "../../base/BaseInternal.h"
#include "ConstantExpression.h"

using namespace lspl::patterns::matchers;
using namespace lspl::text::attributes;

namespace lspl { namespace patterns { namespace expressions {

ConstantExpression::ConstantExpression( const AttributeValue & value ) :
	value( value ) {
}

ConstantExpression::~ConstantExpression() {
}

AttributeValue ConstantExpression::evaluate( const text::Transition * annotation, const Context & ctx ) const {
	return value;
}

void ConstantExpression::dump( std::ostream & out, const std::string & tabs ) const {
	out << value;
}

bool ConstantExpression::equals( const Expression & e ) const {
	if ( const ConstantExpression * exp = dynamic_cast<const ConstantExpression *>( &e ) ) {
		return exp->value == value;
	} else {
		return false;
	}
}

} } } // namespace lspl::patterns::expressions
