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

using lspl::patterns::expressions::Expression;

namespace lspl { namespace patterns { namespace restrictions {

DictionaryRestriction::DictionaryRestriction( const dictionaries::DictionaryConstRef & dict ) :
	dictionary( dict ) {
}

DictionaryRestriction::~DictionaryRestriction() {
}

bool DictionaryRestriction::matches( const text::Transition & annotation, const matchers::Context & ctx ) const {
	std::vector<std::string> words;

	foreach( const Expression & arg, args ) {
		words.push_back( arg.evaluate( &annotation, ctx ).getContainer().getAttribute( AttributeKey::BASE ).getString() );
	}

	return dictionary->accepts( words );
}

void DictionaryRestriction::dump( std::ostream & out, const std::string & tabs ) const {
	out << dictionary->name;
}

bool DictionaryRestriction::equals( const Restriction & r ) const {
	if ( const DictionaryRestriction * dr = dynamic_cast<const DictionaryRestriction *>( &r ) ) {
		if ( dr->dictionary != dictionary ) return false;
		if ( dr->args.size() != args.size() ) return false;

		for ( uint i = 0, l = args.size(); i < l; ++ i )
			if ( !dr->args[i].equals( args[i] ) )
				return false;

		return true;
	} else {
		return false;
	}
}

} } }
