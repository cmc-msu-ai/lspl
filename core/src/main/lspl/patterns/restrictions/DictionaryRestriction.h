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

class LSPL_EXPORT DictionaryRestriction : public Restriction {
public:
	DictionaryRestriction( const dictionaries::DictionaryConstRef & dict, const std::vector<matchers::Variable> & variables, const matchers::Variable & curVar );
	DictionaryRestriction( const dictionaries::DictionaryConstRef & dict, const matchers::Variable & v1, const matchers::Variable & curVar );
	DictionaryRestriction( const dictionaries::DictionaryConstRef & dict, const matchers::Variable & v1, const matchers::Variable & v2, const matchers::Variable & curVar );
	DictionaryRestriction( const dictionaries::DictionaryConstRef & dict, const matchers::Variable & v1, const matchers::Variable & v2, const matchers::Variable & v3, const matchers::Variable & curVar );

	virtual ~DictionaryRestriction();

	bool matches( const text::Transition & annotation, const matchers::Context & ctx ) const;
	void dump( std::ostream & out, const std::string & tabs = "" ) const;
private:
	dictionaries::DictionaryConstRef dictionary;
	std::vector<matchers::Variable> variables;
	matchers::Variable currentVariable;
};

} } }

#endif//_LSPL_PATTERNS_RESTRICTIONS_DICTIONARYRESTRICTION_H_
