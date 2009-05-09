/*
 * Dictionary.cpp
 *
 *  Created on: Sep 27, 2008
 *      Author: alno
 */
#include "../base/BaseInternal.h"

#include "Dictionary.h"

LSPL_REFCOUNT_CLASS( lspl::dictionaries::Dictionary );

namespace lspl { namespace dictionaries {

Dictionary::Dictionary( const std::string & name ) :
	name( name ) {
}

Dictionary::~Dictionary() {
}

bool Dictionary::accepts( const std::string & w1 ) const {
	std::vector<std::string> v;
	v.push_back( w1 );

	return acceptsVector( v );
}

bool Dictionary::accepts( const std::string & w1, const std::string & w2 ) const {
	std::vector<std::string> v;
	v.push_back( w1 );
	v.push_back( w2 );

	return acceptsVector( v );
}

bool Dictionary::accepts( const std::string & w1, const std::string & w2, const std::string & w3 ) const {
	std::vector<std::string> v;
	v.push_back( w1 );
	v.push_back( w2 );
	v.push_back( w3 );

	return acceptsVector( v );
}

} } // namespace lspl::dictionaries
