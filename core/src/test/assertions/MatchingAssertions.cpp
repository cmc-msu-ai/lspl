/*
 * MatchingAssertions.cpp
 *
 *  Created on: Sep 28, 2008
 *      Author: alno
 */

#include "MatchingAssertions.h"
#include "PatternsAssertions.h"

#include <lspl/Namespace.h>
#include <lspl/patterns/Pattern.h>
#include <lspl/patterns/PatternBuilder.h>
#include <lspl/text/Text.h>
#include <lspl/text/readers/PlainTextReader.h>

#include <boost/format.hpp>

using lspl::patterns::PatternRef;
using lspl::text::TextRef;
using lspl::text::MatchList;

namespace lspl { namespace assertions {

void assertMatchesImpl( const NamespaceRef & ns, const char * textSource, uint from, uint to, const char * patternSource, char const *file, int line ) {
	PatternRef pattern = buildPatternImpl( ns, patternSource, file, line );
	TextRef text = buildTextImpl( textSource, file, line );

	const MatchList & matches = text->getMatches( pattern );

	if ( matches.size() == 0 )
		throw cute::test_failure( ( boost::format( "No matches for pattern '%1%' in text '%2%'" ) % patternSource % textSource ).str(), file, line);

	if ( matches[0]->start.index != from || matches[0]->end.index != to )
		throw cute::test_failure( ( boost::format( "Wrong range for pattern '%1%' in text '%2%': [%3%,%4%], but [%5%,%6%] expected" ) % patternSource % textSource % matches[0]->start.index % matches[0]->end.index % from % to ).str(), file, line);
}

void assertNoMatchesImpl( const NamespaceRef & ns, const char * textSource, const char * patternSource, char const *file, int line ) {
	PatternRef pattern = buildPatternImpl( ns, patternSource, file, line );
	TextRef text = buildTextImpl( textSource, file, line );

	const MatchList & matches = text->getMatches( pattern );

	if ( matches.size() != 0 )
		throw cute::test_failure( ( boost::format( "There are matches for pattern '%1%' in text '%2%'" ) % patternSource % textSource ).str(), file, line);
}

text::TextRef buildTextImpl( const char * textSource, char const *file, int line ) {
	text::readers::PlainTextReader reader;
	text::TextRef text = reader.readFromString( textSource );

	if ( !text )
		throw cute::test_failure( ( boost::format( "Error parsing text '%1%'" ) % textSource ).str(), file, line);

	return text;
}

} }
