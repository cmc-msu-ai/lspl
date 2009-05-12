/*
 * PatternsTest.cpp
 *
 *  Created on: Sep 27, 2008
 *      Author: alno
 */

#include "Tests.h"

#include "../assertions/PatternsAssertions.h"

#include <iostream>

#include <lspl/patterns/matchers/Matcher.h>
#include <lspl/patterns/restrictions/Restriction.h>
#include <lspl/dictionaries/MemoryDictionary.h>

using namespace lspl::assertions;

using lspl::patterns::matchers::Matcher;
using lspl::patterns::restrictions::Restriction;

namespace lspl { namespace tests {

void testPatterns() {
	assert( Matcher::aliveObjectsCount == 0 );
	assert( Restriction::aliveObjectsCount == 0 );

	std::cout << "Testing patterns..." << std::endl;

	// Simple
	assertBuilds( "Act = N V" );
	assertBuilds( "Act = N<бизнес> V" );
	assertBuilds( "Act = N<бизнес-вумен> V" );
	assertBuilds( "Act = N<g=fem> V" );
	assertBuilds( "N2<координата, n=plur>" );
	assertFails( "Act1 = N1 N1" ); // Wrong rule naming: Act1
	assertFails( "Act = N1 N1 <3270 0" );
	assertFails( "Act = N1 N1 <3=7>" );
	assertFails( "N2<координата, n=frdscx>" );

	// Names
	assertBuilds( "бизнесвуман = N<бизнес-вумен> V" );
	assertBuilds( "бизнес-вуман = N<бизнес-вумен> V" );

	// Restrictions
	assertBuilds( "Act = N V <N=V>" );
	assertBuilds( "Act = A N V <A=N> <N=V>" );

	// Unnamed
	assertBuilds( "N V" );
	assertBuilds( "N<бизнес> V" );

	// Tokens
	assertBuilds( "\"мама\"" );
	assertBuilds( "'мама'" );
	assertBuilds( "\"м.*ама\"" );
	assertBuilds( "'мам+а'" );

	assertFails( "'мама" );
	assertFails( "\"мама" );

	// Loops
	assertBuilds( "AAA = [ N V ]" );
	assertBuilds( "AAA = { N V }" );
	assertBuilds( "AAA = { N V }<1>" );
	assertBuilds( "AAA = { N V }<1,5>" );
	assertBuilds( "AAA = { N V <N=V> }<1>" );
	assertBuilds( "AAA = { A N V <N=V> <A=N> }<1>" );

	// Alternatives in loops
	assertBuilds( "AAA = [ N | V ]" );
	assertBuilds( "AAA = { N | V }" );
	assertBuilds( "AAA = { N | V }<1>" );
	assertBuilds( "AAA = { N | V }<1,5>" );

	// Parameters
	assertBuilds( "AAA = N V ( N )" );
	assertBuilds( "AAA = N V ( N.c AS d )" );
	assertBuilds( "AAA = N V ( N.c, V.t )" );
	assertBuilds( "AAA = N V ( N.c AS d, V.t AS h )" );

	// Compound attributes
	assertBuilds( "AAA = N V ( N.k.l, V.t )" );
	assertFails( "AAA = N V ( .k.l, V.t )" );

	// Concat
	assertBuilds( "AAA = N V ( N V AS f )" );
	assertBuilds( "AAA = N V ( N V )" );

	// Multiple equal
	assertBuilds("AAA = N1 N2 N3 <N1.c=N2.c=N3.c>");
	assertBuilds("AAA = N1 N2 N3 <N1=N2=N3>");

	// Dictionary
	std::cout << "Testing patterns with dictionaries..." << std::endl;
	{
		NamespaceRef ns = new Namespace();
		ns->addDictionary( new dictionaries::MemoryDictionary("DIC") );
		assertBuilds( ns, "AAA = N V <DIC(N)>" );
		assertBuilds( ns, "AAA = N V <DIC(N,V)>" );
	//	assertFails( ns, "AAA = N V <GG(N)>" );
	}

	std::cout << "Checking memory leaks: ";

	assert( Matcher::aliveObjectsCount == 0 );
	assert( Restriction::aliveObjectsCount == 0 );

	std::cout << "Ok, No memory leaks found" << std::endl;
}

} }
