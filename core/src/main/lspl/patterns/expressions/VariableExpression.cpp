/*
 * VariableExpression.cpp
 *
 *  Created on: Sep 28, 2008
 *      Author: alno
 */
#include "../../base/BaseInternal.h"
#include "VariableExpression.h"
#include "../../text/Transition.h"
#include "../matchers/Context.h"

using namespace lspl::text::attributes;
using namespace lspl::patterns::matchers;

namespace lspl { namespace patterns { namespace expressions {

VariableExpression::VariableExpression( const matchers::Variable & variable ) :
	variable( variable ) {
}

VariableExpression::~VariableExpression() {
}

AttributeValue VariableExpression::evaluate( const text::Transition & annotation, const Context & ctx ) const {
	return ctx.getVariable( variable );
}

void VariableExpression::dump( std::ostream & out, const std::string & tabs ) const {
	out << variable;
}

bool VariableExpression::equals( const Expression & e ) const {
	if ( const VariableExpression * exp = dynamic_cast<const VariableExpression *>( &e ) ) {
		return exp->variable == variable;
	} else {
		return false;
	}
}

} } }
