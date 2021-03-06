/*
 * VariableExpression.h
 *
 *  Created on: Sep 28, 2008
 *      Author: alno
 */

#ifndef _LSPL_PATTERNS_EXPRESSIONS_VARIABLEEXPRESSION_H_
#define _LSPL_PATTERNS_EXPRESSIONS_VARIABLEEXPRESSION_H_

#include <string>

#include "../../base/Base.h"
#include "Expression.h"
#include "../matchers/Variable.h"

namespace lspl { namespace patterns { namespace expressions {

class LSPL_EXPORT VariableExpression : public Expression {
public:
	VariableExpression( const matchers::Variable & variable );
	VariableExpression( text::attributes::SpeechPart sp, uint index );
	VariableExpression( const Pattern & pt, uint index );

	virtual ~VariableExpression();

	virtual void evaluateTo( const text::Transition * currentAnnotation, const matchers::Variable currentVar, const matchers::Context & ctx, ValueList & results ) const;

	virtual void dump( std::ostream & out, const std::string & tabs = "" ) const;
	virtual bool equals( const Expression & e ) const;

	virtual bool containsVariable( matchers::Variable var ) const;
	virtual bool containsVariables() const;
	virtual bool containsCurrentAnnotation() const;

	matchers::Variable getVariable() const {
		return variable;
	}

private:

	matchers::Variable variable;
};

} } }

#endif//_LSPL_PATTERNS_EXPRESSIONS_VARIABLEEXPRESSION_H_
