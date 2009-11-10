/*
 * AgreementRestriction.h
 *
 *  Created on: Sep 28, 2008
 *      Author: alno
 */

#ifndef _LSPL_PATTERNS_RESTRICTIONS_AGREEMENTRESTRICTION_H_
#define _LSPL_PATTERNS_RESTRICTIONS_AGREEMENTRESTRICTION_H_

#include "../../base/Base.h"
#include "Restriction.h"

#include "../matchers/Variable.h"
#include "../expressions/Expression.h"
#include "../../text/attributes/AttributeKey.h"
#include "../../text/attributes/AttributeValue.h"

#include <boost/ptr_container/ptr_vector.hpp>

namespace lspl { namespace patterns { namespace restrictions {

/**
 * Проверка согласования, например <A=N> или <A1.c=N.c>.
 *
 * В процессе работы проверяет согласованность нескольких выражений,
 * вычисленных в рамках контекста сопоставления и текущего кандидата
 * на сопоставленное ребро.
 */
class LSPL_EXPORT AgreementRestriction : public Restriction {
public:
	AgreementRestriction();
	virtual ~AgreementRestriction();

	void addArgument( expressions::Expression * arg ) {
		args.push_back( arg );
	}

	template <class PtrContainer>
	void addArguments( PtrContainer & r ) {
		if ( r.begin() != r.end() )
			args.transfer( args.end(), r.begin(), r.end(), r );
	}

	virtual bool matches( const text::Transition * currentAnnotation, const matchers::Variable currentVar, const matchers::Context & ctx ) const;
	virtual void dump( std::ostream & out, const std::string & tabs = "" ) const;
	virtual bool equals( const Restriction & r ) const;

private:

	/**
	 * Проверить согласование между двумя значениями
	 * @param val1 первое значение
	 * @param val2 второе значение
	 * @return true, если значения согласованы
	 */
	bool checkAgreement( text::attributes::AttributeValue val1, text::attributes::AttributeValue val2 ) const;

private:

	/**
	 * Аргументы согласования
	 */
	boost::ptr_vector<expressions::Expression> args;
};

} } }

#endif//_LSPL_PATTERNS_RESTRICTIONS_AGREEMENTRESTRICTION_H_
