/*
 * DictionariesTest.cpp
 *
 *  Created on: Sep 27, 2008
 *      Author: alno
 */

#include "Tests.h"

#include <iostream>
#include <string>

#include <lspl/dictionaries/MemoryDictionary.h>

namespace lspl { namespace tests {

void testDictionaries() {
	std::cout << "Testing dictionaries..." << std::endl;

	dictionaries::MemoryDictionary d( "aaa" );
	d.add( "aasd", "lnm" );
	d.add( "fds" );

	std::cout << "Memory dictionary: Ok, Built" << std::endl;

	assert( d.accepts( "aasd", "lnm" ) );
	assert( d.accepts( "fds" ) );

	std::cout << "Memory dictionary: Ok, Accepts" << std::endl;

	assert( !d.accepts( "feds", "fds" ) );
	assert( !d.accepts( "aasd" ) );
	assert( !d.accepts( "aasd", "lnm", "vd" ) );
	assert( !d.accepts( "aasd", "fd" ) );
	assert( !d.accepts( "fds", "fds" ) );
	assert( !d.accepts( std::vector<std::string>() ) );

	std::cout << "Memory dictionary: Ok, Declines" << std::endl;
}

} }
