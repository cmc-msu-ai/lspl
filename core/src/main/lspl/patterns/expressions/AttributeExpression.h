/*
 * AttributeExpression.h
 *
 *  Created on: Sep 28, 2008
 *      Author: alno
 */

#ifndef _LSPL_PATTERNS_EXPRESSIONS_ATTRIBUTEEXPRESSION_H_
#define _LSPL_PATTERNS_EXPRESSIONS_ATTRIBUTEEXPRESSION_H_

#include "../../base/Base.h"
#include "Expression.h"
#include "../../text/attributes/AttributeKey.h"

#include <memory>

namespace lspl { namespace patterns { namespace expressions {

class LSPL_EXPORT AttributeExpression : public Expression {
public:
	AttributeExpression( const Expression * base, const text::attributes::AttributeKey & attribute );
	virtual ~AttributeExpression();

	virtual void evaluateTo( const text::Transition * currentAnnotation, const matchers::Variable currentVar, const matchers::Context & ctx, ValueList & results ) const;

	virtual void dump( std::ostream & out, const std::string & tabs = "" ) const;
	virtual bool equals( const Expression & e ) const;

	virtual bool containsVariable( matchers::Variable var ) const;
	virtual bool containsVariables() const;
	virtual bool containsCurrentAnnotation() const;

public:

	const Expression* base;
	text::attributes::AttributeKey attribute;
};

} } }

#endif//_LSPL_PATTERNS_EXPRESSIONS_ATTRIBUTEEXPRESSION_H_
