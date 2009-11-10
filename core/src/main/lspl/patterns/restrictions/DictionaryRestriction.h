/*
 * DictionaryRestriction.h
 *
 *  Created on: Sep 28, 2008
 *      Author: alno
 */

#ifndef _LSPL_PATTERNS_RESTRICTIONS_DICTIONARYRESTRICTION_H_
#define _LSPL_PATTERNS_RESTRICTIONS_DICTIONARYRESTRICTION_H_

#include "../../base/Base.h"
#include "../../dictionaries/Dictionary.h"
#include "Restriction.h"
#include "../matchers/Variable.h"
#include "../expressions/Expression.h"

#include <boost/ptr_container/ptr_vector.hpp>

namespace lspl { namespace patterns { namespace restrictions {

/**
 * Проверка вхождения в словарь, например <Syn(N1,N2)>.
 *
 * При проверке передает значения аргументов классу, реализующему словарь.
 */
class LSPL_EXPORT DictionaryRestriction : public Restriction {
public:
	DictionaryRestriction( const dictionaries::DictionaryConstRef & dict );
	virtual ~DictionaryRestriction();

	void addArgument( expressions::Expression * arg ) {
		args.push_back( arg );
	}

	template <class PtrContainer>
	void addArguments( PtrContainer & r ) {
		if ( r.begin() != r.end() )
			args.transfer( args.end(), r.begin(), r.end(), r );
	}

	virtual bool matches( const text::Transition & annotation, const matchers::Context & ctx ) const;
	virtual void dump( std::ostream & out, const std::string & tabs = "" ) const;
	virtual bool equals( const Restriction & r ) const;

private:

	/**
	 * Словарь, вхождение в который проверяется
	 */
	dictionaries::DictionaryConstRef dictionary;

	/**
	 * Аргументы
	 */
	boost::ptr_vector<expressions::Expression> args;
};

} } }

#endif//_LSPL_PATTERNS_RESTRICTIONS_DICTIONARYRESTRICTION_H_
