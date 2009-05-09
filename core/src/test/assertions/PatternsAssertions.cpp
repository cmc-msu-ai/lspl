/*
 * PatternsAssertions.cpp
 *
 *  Created on: Sep 27, 2008
 *      Author: alno
 */

#include "PatternsAssertions.h"

#include <lspl/Namespace.h>
#include <lspl/patterns/PatternBuilder.h>

#include <iostream>
#include <stdexcept>

namespace lspl { namespace assertions {

void assertBuilds( const NamespaceRef & ns, const std::string & patternSource ) {
	patterns::PatternBuilder builder( ns );

	std::cout << "Checking \"" << patternSource << "\": ";
	std::cout.flush();

	patterns::PatternBuilder::BuildInfo info = builder.build( patternSource );

	if ( info.parseTail.length() > 0 ) {
		std::cout << "ERROR: Pattern was not built: \"" << info.parseTail << "\" not parsed." << std::endl;
		std::cout.flush();

		exit(1);
	} else {
		std::cout << "OK, Built" << std::endl;
	}
}

void assertBuilds( const std::string & patternSource ) {
	assertBuilds( new lspl::Namespace(), patternSource );
}

void assertFails( const std::string & patternSource ) {
	assertFails( new Namespace(), patternSource );
}

void assertFails( const NamespaceRef & ns, const std::string & patternSource ) {
	patterns::PatternBuilder builder( ns );

	std::cout << "Checking \"" + patternSource + "\": ";
	std::cout.flush();

	patterns::PatternBuilder::BuildInfo info;

	try {
		info = builder.build( patternSource );
	} catch ( std::exception & ex ) {
		std::cout << "Ok, not built: " << ex.what() << std::endl;
		return;
	}

	if ( info.parseTail.length() == 0 ) {
		std::cout << "ERROR: Pattern successfully built" << std::endl;
		std::cout.flush();

		exit(1);
	} else {
		std::cout << "OK, Not built" << std::endl;
	}
}

} }
