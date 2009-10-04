/*
 * MatchingText.cpp
 *
 *  Created on: Sep 28, 2008
 *      Author: alno
 */

#include "Tests.h"

#include "../assertions/MatchingAssertions.h"

#include <iostream>
#include <fstream>

#include <lspl/Namespace.h>
#include <lspl/patterns/Pattern.h>
#include <lspl/patterns/PatternBuilder.h>
#include <lspl/text/Text.h>
#include <lspl/text/readers/PlainTextReader.h>
#include <lspl/dictionaries/MemoryDictionary.h>

using namespace lspl::assertions;

namespace lspl { namespace tests {

void testMatching() {
	std::cout << "Testing matching..." << std::endl;

	// Regexp tokens
	assertMatches( "Мама", 0, 1, "'мама'" );
	assertMatches( "Мама", 0, 1, "'ма.*а'" );
	assertMatches( "Маша", 0, 1, "'ма.*а'" );
	assertMatches( "Мамаша", 0, 1, "'ма.*а'" );
	assertNoMatches( "Мошка", "'ма.*а'" );

	// Simple
	assertMatches( "Мама мыла раму", 0, 2, "Act = N V" );
	assertMatches( "Мама мыла раму", 0, 2, "Act = N<мама> V" );
	assertMatches( "Мама мыла раму", 0, 2, "Act = N V<N=V>" );
	assertMatches( "Мама мыла раму", 0, 2, "Act = N { V<N=V> }<1,1>" );
	assertMatches( "Мама мыла раму", 0, 2, "Act = N { { V<N=V> }<1,1> }<1,1>" );
	assertMatches( "Мама мыла раму", 0, 2, "Act = N V<N.g=V.g>" );
	assertMatches( "Мама мыла раму", 0, 2, "Act = N<g=fem> V" );
	assertMatches( "Мама мыла раму", 0, 2, "Act = \"мама\" V" );
	assertNoMatches( "Мама мыла раму", "Act = N<папа> V" );

	// Term
	assertMatches( "Процессор базы данных", 0, 3, "\"процессор\" \"базы\" \"данных\"" );
	assertMatches( "Процессор базы данных", 0, 3, "N1<процессор> { \"базы\" \"данных\" | \"ввода-вывода\" }<1,1>" );

	// Loop restrictions: positive
	assertMatches( "Мама мыла раму", 0, 2, "Act = {N} V" );
	assertMatches( "Мама мыла раму", 0, 2, "Act = {N} V<N=V>" );
	assertMatches( "Мама мыла раму", 0, 2, "Act = {N} V<N.g=V.g>" );

	// Loop restriction: empty loop
	assertMatches( "Мама мыла раму", 0, 1, "Act = [A] N" );
	assertMatches( "Мама мыла раму", 0, 1, "Act = {A} N" );
	assertMatches( "Мама мыла раму", 0, 1, "Act = {A} N <A=N>" );

	// Multiple equality: positive
	assertMatches( "Белая кошка мыла раму", 0, 3, "Act = A N V<A=N=V>" );
	assertMatches( "Белая кошка мыла раму", 0, 3, "Act = A N V<A.g=N.g=V.g>" );

	// Multiple equality: negative
	assertNoMatches( "Белый кошка мыла раму", "Act = A N V<A=N=V>" );
	assertNoMatches( "Белая кот мыла раму", "Act = A N V<A=N=V>" );
	assertNoMatches( "Белая кошка мыл раму", "Act = A N V<A=N=V>" );
	assertNoMatches( "Белый кошка мыла раму", "Act = A N V<A.g=N.g=V.g>" );

	// Base equality
	assertMatches( "Мама, мама, что я буду делать?", 0, 3, "Act = W1 \",\" W2 <W1.b=W2.b>" );

	assertNoMatches( "Кот прыгнул на стол", "Act = N<g=fem> V" );
	assertNoMatches( "Кот прыгнула на стол", "Act = N V<N=V>" );
	assertNoMatches( "Кот прыгнул на стол", "Act = W1 W2 <W1.b=W2.b>" );

	assertMatches( "Мама мыла раму", 0, 2, "Act = N V<N=V> ( N V )" );

	std::cout << "Testing simple reusing..." << std::endl;
	{
		NamespaceRef ns = new Namespace();

		assertMatches( ns, "Белая кошка шла по дорожке", 0, 1, "AA = A (A) | Pa (Pa)" );
		assertMatches( ns, "Белая кошка шла по дорожке", 0, 2, "TestA = AA N" );
		assertMatches( ns, "Белая кошка шла по дорожке", 0, 2, "TestB = AA N <AA=N>" );
	}
	{
		NamespaceRef ns = new Namespace();

		assertNoMatches( ns, "Мама мыла раму", "AA = A (A) | Pa (Pa)" );
		assertMatches( ns, "Мама мыла раму", 0, 1, "TestA = {AA} N" );
		assertMatches( ns, "Мама мыла раму", 0, 1, "TestB = {AA} N <Ap=N>" );
	}

	std::cout << "Testing matching in one namespace..." << std::endl;
	{
		NamespaceRef ns = new Namespace();

		assertMatches( ns, "Мама мыла раму", 0, 1, "UN = N" );
		assertMatches( ns, "Мама мыла раму", 0, 2, "AB = UN V" );
		assertMatches( ns, "Мама мыла раму", 0, 2, "AC = UN V <UN=V>" );
	}

	std::cout << "Testing matching with compound attributes..." << std::endl;
	{
		NamespaceRef ns = new Namespace();
		patterns::PatternBuilderRef builder = new patterns::PatternBuilder( ns );

		builder->build( "UV = V (V AS src)" );
		builder->build( "UUV = UV ( UV.src )" );

		assertMatches( ns, "Мама мыла раму", 0, 2, "AA = N UV (UV)" );
		assertMatches( ns, "Мама мыла раму", 0, 2, "AB = N UUV <N=UUV> (UUV)" );
		assertMatches( ns, "Кот мыла раму", 0, 2, "AC = N UUV <N=UUV> (UUV)" ); // TODO Вот такой вот парадокс
	}

	std::cout << "Testing matching with dictionaries..." << std::endl;
	{
		base::Reference<dictionaries::MemoryDictionary> d = new dictionaries::MemoryDictionary( "DIC" );
		d->add( "МЫТЬ", "РАМА" );

		NamespaceRef ns = new Namespace();
		ns->addDictionary( d );

		assertMatches( ns, "Мама мыла раму", 1, 3, "AA = W1 W2 <DIC(W1,W2)>" );
		assertNoMatches( ns, "Мама мыла окно", "AB = W1 W2 <DIC(W1,W2)>" );
	}
}

} }
