/*
 * VariableExpression.h
 *
 *  Created on: Sep 28, 2008
 *      Author: alno
 */

#ifndef _LSPL_PATTERNS_EXPRESSIONS_VARIABLEEXPRESSION_H_
#define _LSPL_PATTERNS_EXPRESSIONS_VARIABLEEXPRESSION_H_

#include "../../base/Base.h"
#include "Expression.h"
#include "../matchers/Variable.h"

namespace lspl { namespace patterns { namespace expressions {

class VariableExpression : public Expression {
public:
	VariableExpression( const matchers::Variable & variable );
	virtual ~VariableExpression();

	virtual text::attributes::AttributeValue evaluate( const text::Transition & annotation, const matchers::Context & ctx ) const;

	virtual void dump( std::ostream & out, const std::string & tabs = "" ) const;
	virtual bool equals( const Expression & e ) const;

private:

	matchers::Variable variable;
};

} } }

#endif//_LSPL_PATTERNS_EXPRESSIONS_VARIABLEEXPRESSION_H_
