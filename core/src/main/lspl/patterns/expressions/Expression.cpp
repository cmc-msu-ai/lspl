/*
 * Expression.cpp
 *
 *  Created on: Sep 28, 2008
 *      Author: alno
 */
#include "../../base/BaseInternal.h"
#include "Expression.h"

namespace lspl { namespace patterns { namespace expressions {

Expression::Expression() {
}

Expression::~Expression() {
}

Expression::ValueListPtr Expression::evaluate( const text::Transition * currentAnnotation, const matchers::Variable currentVar, const matchers::Context & ctx ) const {
	ValueListPtr results( new ValueList() );

	evaluateTo( currentAnnotation, currentVar, ctx, *results );

	return results;
}

} } }
