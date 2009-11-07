/*
 * DictionariesTest.cpp
 *
 *  Created on: Sep 27, 2008
 *      Author: alno
 */

#include "Tests.h"
#include "../assertions/PatternsAssertions.h"

#include <iostream>
#include <string>

#include <lspl/dictionaries/MemoryDictionary.h>

namespace lspl { namespace tests {

static void testMemoryDictionary() {
	dictionaries::MemoryDictionary d( "aaa" );
	d.add( "aasd", "lnm" );
	d.add( "fds" );

	assertTrue( d.accepts( "aasd", "lnm" ) );
	assertTrue( d.accepts( "fds" ) );

	assertFalse( d.accepts( "feds", "fds" ) );
	assertFalse( d.accepts( "aasd" ) );
	assertFalse( d.accepts( "aasd", "lnm", "vd" ) );
	assertFalse( d.accepts( "aasd", "fd" ) );
	assertFalse( d.accepts( "fds", "fds" ) );
	assertFalse( d.accepts( std::vector<std::string>() ) );
}

cute::suite dictionariesSuite() {
	cute::suite s;

	s += CUTE(testMemoryDictionary);

	return s;
}

} }
