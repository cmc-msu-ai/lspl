/*
 * MatchingAssertions.cpp
 *
 *  Created on: Sep 28, 2008
 *      Author: alno
 */

#include "MatchingAssertions.h"

#include <lspl/Namespace.h>
#include <lspl/patterns/Pattern.h>
#include <lspl/patterns/PatternBuilder.h>
#include <lspl/text/Text.h>
#include <lspl/text/readers/PlainTextReader.h>

#include <cstdlib>
#include <iostream>

namespace lspl {

namespace assertions {

void assertMatches( const char * textSource, uint from, uint to, const char * patternSource ) {
	assertMatches( new Namespace(), textSource, from, to, patternSource );
}

void assertMatches( const NamespaceRef & ns, const char * textSource, uint from, uint to, const char * patternSource ) {
	patterns::PatternBuilder builder( ns );

	std::cout << "Checking matching \"" << patternSource << "\": ";
	std::cout.flush();

	patterns::PatternBuilder::BuildInfo info = builder.build( patternSource );

	if ( info.parseTail.length() > 0 ) {
		std::cout << "ERROR: Pattern was not built: \"" << info.parseTail << "\" not parsed." << std::endl;
		std::cout.flush();

		exit(1);
	}

	text::readers::PlainTextReader reader;
	text::TextRef text = reader.readFromString( textSource );

	assert( text );

	const std::vector< text::MatchRef > & matches = text->getMatches( ns->getPatternByIndex( ns->getPatternCount() - 1 ) );

	assert( matches.size() > 0 );
	assert( matches[0]->start.index == from );
	assert( matches[0]->end.index == to );
	assert( matches[0]->getVariants()[0].getTransformResult<int>() == 0 );

	std::cout << "Ok, matched" << std::endl;
}

void assertNoMatches( const char * textSource, const char * patternSource ) {
	assertNoMatches( new Namespace(), textSource, patternSource );
}

void assertNoMatches( const NamespaceRef & ns, const char * textSource, const char * patternSource ) {
	patterns::PatternBuilder builder( ns );

	std::cout << "Checking matching \"" << patternSource << "\": ";
	std::cout.flush();

	patterns::PatternBuilder::BuildInfo info = builder.build( patternSource );

	if ( info.parseTail.length() > 0 ) {
		std::cout << "ERROR: Pattern was not built: \"" << info.parseTail << "\" not parsed." << std::endl;
		std::cout.flush();

		exit(1);
	}

	text::readers::PlainTextReader reader;
	text::TextRef text = reader.readFromString( textSource );

	assert( text );
	assert( text->getMatches( ns->getPatternByIndex( ns->getPatternCount() - 1 ) ).size() == 0 );

	std::cout << "Ok, No matches" << std::endl;
}

} }
