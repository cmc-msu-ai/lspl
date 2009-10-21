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

#include <vector>

namespace lspl { namespace patterns { namespace restrictions {

/**
 * Проверка вхождения в словарь, например <Syn(N1,N2)>.
 *
 * При проверке передает значения аргументов классу, реализующему словарь.
 */
class LSPL_EXPORT DictionaryRestriction : public Restriction {
public:
	DictionaryRestriction( const dictionaries::DictionaryConstRef & dict, const std::vector<matchers::Variable> & variables, const matchers::Variable & curVar );
	DictionaryRestriction( const dictionaries::DictionaryConstRef & dict, const matchers::Variable & v1, const matchers::Variable & curVar );
	DictionaryRestriction( const dictionaries::DictionaryConstRef & dict, const matchers::Variable & v1, const matchers::Variable & v2, const matchers::Variable & curVar );
	DictionaryRestriction( const dictionaries::DictionaryConstRef & dict, const matchers::Variable & v1, const matchers::Variable & v2, const matchers::Variable & v3, const matchers::Variable & curVar );

	virtual ~DictionaryRestriction();

	virtual bool matches( const text::Transition & annotation, const matchers::Context & ctx ) const;
	virtual void dump( std::ostream & out, const std::string & tabs = "" ) const;
	virtual bool equals( const Restriction & r ) const;

private:

	/**
	 * Словарь, вхождение в который проверяется
	 */
	dictionaries::DictionaryConstRef dictionary;

	/**
	 * Переменный, вхождение которых проверяется
	 */
	std::vector<matchers::Variable> variables;

	/**
	 * Переменная, соответствующая текущему элементу шаблона
	 */
	matchers::Variable currentVariable;
};

} } }

#endif//_LSPL_PATTERNS_RESTRICTIONS_DICTIONARYRESTRICTION_H_
