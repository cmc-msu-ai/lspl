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

#include <boost/format.hpp>

namespace lspl {

namespace assertions {

void assertMatchesImpl( const char * textSource, uint from, uint to, const char * patternSource, char const *f, int line ) {
	assertMatchesImpl( new Namespace(), textSource, from, to, patternSource, f, line );
}

void assertMatchesImpl( const NamespaceRef & ns, const char * textSource, uint from, uint to, const char * patternSource, char const *file, int line ) {
	patterns::PatternBuilder builder( ns );
	patterns::PatternBuilder::BuildInfo info = builder.build( patternSource );

	if ( info.parseTail.length() > 0 )
		throw cute::test_failure( ( boost::format( "Pattern '%1%' was not built: '%2%' not parsed." ) % patternSource % info.parseTail ).str(), file, line);

	text::readers::PlainTextReader reader;
	text::TextRef text = reader.readFromString( textSource );

	if ( !text )
		throw cute::test_failure( ( boost::format( "Error parsing text '%1%'" ) % textSource ).str(), file, line);

	const std::vector< text::MatchRef > & matches = text->getMatches( ns->getPatternByIndex( ns->getPatternCount() - 1 ) );

	if ( matches.size() == 0 )
		throw cute::test_failure( ( boost::format( "No matches for pattern '%1%' in text '%2%'" ) % patternSource % textSource ).str(), file, line);

	if ( matches[0]->start.index != from || matches[0]->end.index != to )
		throw cute::test_failure( ( boost::format( "Wrong range for pattern '%1%' in text '%2%': [%3%,%4%], but [%5%,%6%] expected" ) % patternSource % textSource % matches[0]->start.index % matches[0]->end.index % from % to ).str(), file, line);
}

void assertNoMatchesImpl( const char * textSource, const char * patternSource, char const *f, int line ) {
	assertNoMatchesImpl( new Namespace(), textSource, patternSource, f, line );
}

void assertNoMatchesImpl( const NamespaceRef & ns, const char * textSource, const char * patternSource, char const *file, int line ) {
	patterns::PatternBuilder builder( ns );
	patterns::PatternBuilder::BuildInfo info = builder.build( patternSource );

	if ( info.parseTail.length() > 0 )
		throw cute::test_failure( ( boost::format( "ERROR: Pattern '%1%' was not built: '%2%' not parsed." ) % patternSource % info.parseTail ).str(), file, line);

	text::readers::PlainTextReader reader;
	text::TextRef text = reader.readFromString( textSource );

	if ( !text )
		throw cute::test_failure( ( boost::format( "Error parsing text '%1%'" ) % textSource ).str(), file, line);

	const std::vector< text::MatchRef > & matches = text->getMatches( ns->getPatternByIndex( ns->getPatternCount() - 1 ) );

	if ( matches.size() != 0 )
		throw cute::test_failure( ( boost::format( "There are matches for pattern '%1%' in text '%2%'" ) % patternSource % textSource ).str(), file, line);
}

} }
