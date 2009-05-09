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

namespace lspl { namespace assertions {

void assertBuilds( const NamespaceRef & ns, const std::string & patternSource );
void assertBuilds( const std::string & patternSource );
void assertFails( const NamespaceRef & ns, const std::string & patternSource );
void assertFails( const std::string & patternSource );

} }

#endif//_LSPL_ASSERTIONS_PATTERNSASSERTIONS_H_
