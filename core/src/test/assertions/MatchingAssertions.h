/*
 * MatchingAssertions.h
 *
 *  Created on: Sep 28, 2008
 *      Author: alno
 */

#ifndef MATCHINGASSERTIONS_H_
#define MATCHINGASSERTIONS_H_

#undef NDEBUG

#include <lspl/Namespace.h>

namespace lspl {

namespace assertions {

void assertMatches( const char * textSource, uint from, uint to, const char * patternSource );

void assertMatches( const NamespaceRef & ns, const char * textSource, uint from, uint to, const char * patternSource );

void assertNoMatches( const char * textSource, const char * patternSource );

void assertNoMatches( const NamespaceRef & ns, const char * textSource, const char * patternSource );

}

}

#endif /* MATCHINGASSERTIONS_H_ */
