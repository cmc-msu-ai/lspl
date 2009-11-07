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

namespace lspl { namespace assertions {

void assertBuildsImpl( const NamespaceRef & ns, const std::string & patternSource, char const *file, int line ) {
	patterns::PatternBuilder builder( ns );
	patterns::PatternBuilder::BuildInfo info = builder.build( patternSource );

	if ( info.parseTail.length() > 0 )
		throw cute::test_failure( ( boost::format( "ERROR: Pattern '%1%' was not built: '%2%' not parsed." ) % patternSource % info.parseTail ).str(), file, line);
}

void assertBuildsImpl( const std::string & patternSource, char const *f, int line ) {
	assertBuildsImpl( new lspl::Namespace(), patternSource, f, line );
}

void assertFailsImpl( const std::string & patternSource, char const *f, int line ) {
	assertFailsImpl( new Namespace(), patternSource, f, line );
}

void assertFailsImpl( const NamespaceRef & ns, const std::string & patternSource, char const *file, int line ) {
	patterns::PatternBuilder builder( ns );
	patterns::PatternBuilder::BuildInfo info;

	try {
		info = builder.build( patternSource );
	} catch ( std::exception & ex ) {
		return;
	}

	if ( info.parseTail.length() == 0 )
		throw cute::test_failure( ( boost::format( "ERROR: Pattern '%1%' successfully built." ) % patternSource ).str(), file, line);
}

} }
