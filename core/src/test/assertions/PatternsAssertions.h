/*
 * PatternsAssertions.h
 *
 *  Created on: Sep 27, 2008
 *      Author: alno
 */

#ifndef _LSPL_ASSERTIONS_PATTERNSASSERTIONS_H_
#define _LSPL_ASSERTIONS_PATTERNSASSERTIONS_H_

#undef NDEBUG

#include "../../main/lspl/Namespace.h"

#include "../../cute/cute.h"

#include <string>
#include <iostream>

#define assertTrueM(exp,msg) if (!(exp)) throw cute::test_failure((msg),__FILE__,__LINE__)
#define assertTrue(exp) assertTrueM(exp,#exp)

#define assertFalseM(exp,msg) if ((exp)) throw cute::test_failure((msg),__FILE__,__LINE__)
#define assertFalse(exp) assertFalseM(exp,#exp)

#define assertEqualsM(a,b,msg) cute::assert_equal((b),(a),msg,__FILE__,__LINE__)
#define assertEquals(a,b) assertEqualsM(a,b,#a " == " #b)

#define assertBuildsNS(pattern,ns) lspl::assertions::assertBuildsImpl(ns,pattern,__FILE__,__LINE__)
#define assertBuilds(pattern) lspl::assertions::assertBuildsImpl(new lspl::Namespace(),pattern,__FILE__,__LINE__)
#define assertFailsNS(pattern,ns) lspl::assertions::assertFailsImpl(ns,pattern,__FILE__,__LINE__)
#define assertFails(pattern) lspl::assertions::assertFailsImpl(new lspl::Namespace(),pattern,__FILE__,__LINE__)

#define buildPatternNS(pattern,ns) lspl::assertions::buildPatternImpl(ns,pattern,__FILE__,__LINE__)
#define buildPattern(pattern) lspl::assertions::buildPatternImpl(new lspl::Namespace(),pattern,__FILE__,__LINE__)

namespace lspl { namespace assertions {

void assertBuildsImpl( const NamespaceRef & ns, const std::string & patternSource, char const *f, int line );
void assertFailsImpl( const NamespaceRef & ns, const std::string & patternSource, char const *f, int line );

patterns::PatternRef buildPatternImpl( const NamespaceRef & ns, const std::string & patternSource, char const *f, int line );

} }

#endif//_LSPL_ASSERTIONS_PATTERNSASSERTIONS_H_
