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

AttributeValue CurrentAnnotationExpression::evaluate( const text::Transition & annotation, const Context & ctx ) const {
	return AttributeValue( annotation );
}

void CurrentAnnotationExpression::dump( std::ostream & out, const std::string & tabs ) const {
	out << "$";
}

} } }
