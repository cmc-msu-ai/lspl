/*
 * ConstantExpression.h
 *
 *  Created on: Sep 28, 2008
 *      Author: alno
 */

#ifndef _LSPL_PATTERNS_EXPRESSIONS_CONSTANTEXPRESSION_H_
#define _LSPL_PATTERNS_EXPRESSIONS_CONSTANTEXPRESSION_H_

#include "../../base/Base.h"
#include "Expression.h"

namespace lspl { namespace patterns { namespace expressions {

class LSPL_EXPORT ConstantExpression : public Expression {
public:
	ConstantExpression( const text::attributes::AttributeValue & value );
	virtual ~ConstantExpression();

	virtual text::attributes::AttributeValue evaluate( const text::Transition * currentAnnotation, const matchers::Variable currentVar, const matchers::Context & ctx ) const;

	virtual void dump( std::ostream & out, const std::string & tabs = "" ) const;
	virtual bool equals( const Expression & e ) const;
	virtual bool containsVariable( matchers::Variable var ) const;

private:

	text::attributes::AttributeValue value;
};

} } }

#endif//_LSPL_PATTERNS_EXPRESSIONS_CONSTANTEXPRESSION_H_
