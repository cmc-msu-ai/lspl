/*
 * CurrentAnnotationExpressionExpression.cpp
 *
 *  Created on: Nov 3, 2008
 *      Author: alno
 */
#include "../../base/BaseInternal.h"
#include "CurrentAnnotationExpression.h"
#include "../matchers/Context.h"
#include "../../text/Transition.h"

using namespace lspl::text::attributes;
using namespace lspl::patterns::matchers;

namespace lspl { namespace patterns { namespace expressions {

CurrentAnnotationExpression::CurrentAnnotationExpression() {
}

CurrentAnnotationExpression::~CurrentAnnotationExpression() {
}

void CurrentAnnotationExpression::evaluateTo( const text::Transition * currentAnnotation, const matchers::Variable currentVar, const Context & ctx, ValueList & results ) const {
	results.push_back( AttributeValue( *currentAnnotation ) );
}

void CurrentAnnotationExpression::dump( std::ostream & out, const std::string & tabs ) const {
	out << "$";
}

bool CurrentAnnotationExpression::equals( const Expression & e ) const {
	if ( const CurrentAnnotationExpression * exp = dynamic_cast<const CurrentAnnotationExpression *>( &e ) ) {
		return true;
	} else {
		return false;
	}
}

bool CurrentAnnotationExpression::containsVariable( matchers::Variable var ) const {
	return false;
}

bool CurrentAnnotationExpression::containsVariables() const {
	return false;
}

bool CurrentAnnotationExpression::containsCurrentAnnotation() const {
	return true;
}

} } }
