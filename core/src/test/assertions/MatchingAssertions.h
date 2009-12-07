/*
 * MatchingAssertions.h
 *
 *  Created on: Sep 28, 2008
 *      Author: alno
 */

#ifndef MATCHINGASSERTIONS_H_
#define MATCHINGASSERTIONS_H_

#undef NDEBUG

#include "../../main/lspl/Namespace.h"
#include "../../main/lspl/text/Forward.h"

#include "../../main/lspl/patterns/restrictions/Restriction.h"

#include "../../cute/cute.h"

#define assertMatches(text,from,to,pattern) lspl::assertions::assertMatchesImpl(new lspl::Namespace(),text,from,to,pattern,__FILE__,__LINE__)
#define assertMatchesNS(ns,text,from,to,pattern) lspl::assertions::assertMatchesImpl(ns,text,from,to,pattern,__FILE__,__LINE__)

#define assertNoMatches(text,pattern) lspl::assertions::assertNoMatchesImpl(new lspl::Namespace(),text,pattern,__FILE__,__LINE__)
#define assertNoMatchesNS(ns,text,pattern) lspl::assertions::assertNoMatchesImpl(ns,text,pattern,__FILE__,__LINE__)

#define assertRestrictedMatches(text,from,to,pattern,r) lspl::assertions::assertRestrictedMatchesImpl(new lspl::Namespace(),text,from,to,pattern,r,__FILE__,__LINE__)
#define assertRestrictedMatchesNS(ns,text,from,to,pattern,r) lspl::assertions::assertRestrictedMatchesImpl(ns,text,from,to,pattern,r,__FILE__,__LINE__)

#define assertNoRestrictedMatches(text,pattern,r) lspl::assertions::assertNoRestrictedMatchesImpl(new lspl::Namespace(),text,pattern,r,__FILE__,__LINE__)
#define assertNoRestrictedMatchesNS(ns,text,pattern,r) lspl::assertions::assertNoRestrictedMatchesImpl(ns,text,pattern,r,__FILE__,__LINE__)

#define buildText(text) lspl::assertions::buildTextImpl(text,__FILE__,__LINE__)

namespace lspl { namespace assertions {

void assertMatchesImpl( const NamespaceRef & ns, const char * textSource, uint from, uint to, const char * patternSource, char const *f, int line );
void assertNoMatchesImpl( const NamespaceRef & ns, const char * textSource, const char * patternSource, char const *f, int line );

void assertRestrictedMatchesImpl( const NamespaceRef & ns, const char * textSource, uint from, uint to, const char * patternSource, const patterns::restrictions::Restriction & r, char const *f, int line );
void assertNoRestrictedMatchesImpl( const NamespaceRef & ns, const char * textSource, const char * patternSource, const patterns::restrictions::Restriction & r, char const *f, int line );

text::TextRef buildTextImpl( const char * textSource, char const *file, int line );

} }

#endif /* MATCHINGASSERTIONS_H_ */
