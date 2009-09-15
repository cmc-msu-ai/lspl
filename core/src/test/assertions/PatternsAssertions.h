/*
 * PatternsAssertions.h
 *
 *  Created on: Sep 27, 2008
 *      Author: alno
 */

#ifndef _LSPL_ASSERTIONS_PATTERNSASSERTIONS_H_
#define _LSPL_ASSERTIONS_PATTERNSASSERTIONS_H_

#undef NDEBUG

#include <lspl/Namespace.h>

#include <string>
#include <iostream>

namespace lspl { namespace assertions {

void assertBuilds( const NamespaceRef & ns, const std::string & patternSource );
void assertBuilds( const std::string & patternSource );
void assertFails( const NamespaceRef & ns, const std::string & patternSource );
void assertFails( const std::string & patternSource );

void assertFalse( bool exp, const char * message = 0);
void assertTrue( bool exp, const char * message = 0);

template< typename A, typename B >
void assertEquals( A a, B b ) {
	if ( a != b ) {
		std::cout << "ERROR: " << a << " expected, but got " << b << std::endl;
		std::cout.flush();

		exit(1);
	}
}

} }

#endif//_LSPL_ASSERTIONS_PATTERNSASSERTIONS_H_
