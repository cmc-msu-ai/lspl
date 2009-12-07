/*
 * PatternsAssertions.cpp
 *
 *  Created on: Sep 27, 2008
 *      Author: alno
 */

#include "PatternsAssertions.h"

#include <lspl/Namespace.h>
#include <lspl/patterns/Pattern.h>
#include <lspl/patterns/PatternBuilder.h>

#include <stdexcept>

#include <boost/format.hpp>

using namespace lspl::patterns;

namespace lspl { namespace assertions {

void assertBuildsImpl( const NamespaceRef & ns, const std::string & patternSource, char const *file, int line ) {
	PatternBuilder builder( ns );
	PatternBuilder::BuildInfo info;

	try {
		info = builder.build( patternSource );
	} catch ( const std::exception & e ) {
		throw cute::test_failure( ( boost::format( "ERROR: %1%" ) % e.what() ).str(), file, line);
	} catch (...) {
		throw cute::test_failure( "ERROR: Unknown exception", file, line);
	}

	if ( info.parseTail.length() > 0 )
		throw cute::test_failure( ( boost::format( "ERROR: Pattern '%1%' was not built: '%2%' not parsed." ) % patternSource % info.parseTail ).str(), file, line);
}

void assertFailsImpl( const NamespaceRef & ns, const std::string & patternSource, char const *file, int line ) {
	PatternBuilder builder( ns );
	PatternBuilder::BuildInfo info;

	try {
		info = builder.build( patternSource );
	} catch ( std::exception & ex ) {
		return;
	} catch (...) {
		throw cute::test_failure( "ERROR: Unknown exception", file, line);
	}

	if ( info.parseTail.length() == 0 )
		throw cute::test_failure( ( boost::format( "ERROR: Pattern '%1%' successfully built." ) % patternSource ).str(), file, line);
}

PatternRef buildPatternImpl( const NamespaceRef & ns, const std::string & patternSource, char const *f, int line ) {
	assertBuildsImpl( ns, patternSource, f, line );

	return ns->getPatternByIndex( ns->getPatternCount() - 1 );
}

} }
