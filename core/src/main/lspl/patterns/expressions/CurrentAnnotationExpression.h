/*
 * CurrentAnnotationExpression.h
 *
 *  Created on: Nov 3, 2008
 *      Author: alno
 */

#ifndef _LSPL_PATTERNS_EXPRESSIONS_CURRENTANNOTATIONEXPRESSION_H_
#define _LSPL_PATTERNS_EXPRESSIONS_CURRENTANNOTATIONEXPRESSION_H_

#include "../../base/Base.h"
#include "Expression.h"
#include "../matchers/Variable.h"

namespace lspl { namespace patterns { namespace expressions {

class CurrentAnnotationExpression : public Expression {
public:
	CurrentAnnotationExpression();
	virtual ~CurrentAnnotationExpression();

	text::attributes::AttributeValue evaluate( const text::Transition & annotation, const matchers::Context & ctx ) const;

	void dump( std::ostream & out, const std::string & tabs = "" ) const;
};

} } }

#endif//_LSPL_PATTERNS_EXPRESSIONS_CURRENTANNOTATIONEXPRESSION_H_
