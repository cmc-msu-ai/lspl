/*
 * TextReadersTest.cpp
 *
 *  Created on: Sep 28, 2008
 *      Author: alno
 */

#include "Tests.h"

#include <iostream>
#include <fstream>

#include <lspl/text/readers/PlainTextReader.h>
#include <lspl/text/readers/JsonTextReader.h>

namespace lspl { namespace tests {

void assertLoadsJson( const char * fileName ) {
	std::ifstream is( fileName );
	lspl::text::readers::JsonTextReader reader( new lspl::Namespace() );

	std::cout << "Checking JSON loading from file \"" << fileName << "\": ";
	std::cout.flush();

	assert( reader.readFromStream( is ) );

	std::cout << "Ok, Loaded" << std::endl;

	is.close();
}

void assertBuilds( const char * text ) {
	lspl::text::readers::PlainTextReader reader;

	std::cout << "Checking building text: ";
	std::cout.flush();

	assert( reader.readFromString( text ) );

	std::cout << "Ok, Built" << std::endl;
}

static void testTextReaders() {
	assertLoadsJson( "resources/test/text1.js" );

	assertBuilds( "Мама мыла раму" );
}

cute::suite textReadersSuite() {
	cute::suite s;

	s += CUTE(testTextReaders);

	return s;
}

} }
