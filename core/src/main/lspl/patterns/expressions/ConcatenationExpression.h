/*
 * ConcatentaionExpression.h
 *
 *  Created on: Sep 30, 2008
 *      Author: alno
 */

#ifndef _LSPL_PATTERNS_EXPRESSIONS_CONCATENTAIONEXPRESSION_H_
#define _LSPL_PATTERNS_EXPRESSIONS_CONCATENTAIONEXPRESSION_H_

#include "../../base/Base.h"
#include "Expression.h"
#include "../../text/attributes/AttributeKey.h"

#include <boost/ptr_container/ptr_vector.hpp>

namespace lspl { namespace patterns { namespace expressions {

class ConcatenationExpression : public Expression {
public:
	ConcatenationExpression() {}

	template <class PtrContainer>
	ConcatenationExpression( PtrContainer & args ) {
		addArguments( args );
	}

	virtual ~ConcatenationExpression();

	void addArgument( Expression * arg ) {
		args.push_back( arg );
	}

	template <class PtrContainer>
	void addArguments( PtrContainer & r ) {
		if ( r.begin() != r.end() )
			args.transfer( args.end(), r.begin(), r.end(), r );
	}

	text::attributes::AttributeValue evaluate( const text::Transition & annotation, const matchers::Context & ctx ) const;

	void dump( std::ostream & out, const std::string & tabs = "" ) const;
public:
	boost::ptr_vector<Expression> args;
};

} } }

#endif//_LSPL_PATTERNS_EXPRESSIONS_CONCATENTAIONEXPRESSION_H_
