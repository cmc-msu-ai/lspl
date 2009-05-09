/*
 * DictionaryRestriction.cpp
 *
 *  Created on: Sep 28, 2008
 *      Author: alno
 */
#include "../../base/BaseInternal.h"
#include "../../text/Transition.h"
#include "../../text/attributes/AttributeKey.h"
#include "../matchers/Context.h"
#include "DictionaryRestriction.h"

using namespace lspl::text::attributes;

namespace lspl { namespace patterns { namespace restrictions {

DictionaryRestriction::DictionaryRestriction( const dictionaries::DictionaryConstRef & dict, const std::vector<matchers::Variable> & variables, const matchers::Variable & curVar ) :
	dictionary( dict ), variables( variables ), currentVariable( curVar ) {
}

DictionaryRestriction::DictionaryRestriction( const dictionaries::DictionaryConstRef & dict, const matchers::Variable & v1, const matchers::Variable & curVar ) :
	dictionary( dict ), currentVariable( curVar ) {
	variables.push_back( v1 );
}

DictionaryRestriction::DictionaryRestriction( const dictionaries::DictionaryConstRef & dict, const matchers::Variable & v1, const matchers::Variable & v2, const matchers::Variable & curVar ) :
	dictionary( dict ), currentVariable( curVar ) {
	variables.push_back( v1 );
	variables.push_back( v2 );
}

DictionaryRestriction::DictionaryRestriction( const dictionaries::DictionaryConstRef & dict, const matchers::Variable & v1, const matchers::Variable & v2, const matchers::Variable & v3, const matchers::Variable & curVar ) :
	dictionary( dict ), currentVariable( curVar ) {
	variables.push_back( v1 );
	variables.push_back( v2 );
	variables.push_back( v3 );
}

DictionaryRestriction::~DictionaryRestriction() {
}

bool DictionaryRestriction::matches( const text::Transition & annotation, const matchers::Context & ctx ) const {
	std::vector<std::string> words;

	for ( uint i = 0; i < variables.size(); ++ i ) {
		matchers::Variable v = variables[i];

		if ( v == currentVariable )
			words.push_back( annotation.getAttribute( AttributeKey::BASE ).getString() );
		else
			words.push_back( ctx.getAttribute( variables[i], AttributeKey::BASE ).getString() );
	}

	return dictionary->accepts( words );
}

void DictionaryRestriction::dump( std::ostream & out, const std::string & tabs ) const {
	out << variables[0];
}

} } }
