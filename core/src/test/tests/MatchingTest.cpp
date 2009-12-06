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

static void testRegexpTokens() {
	// Regexp tokens
	assertMatches( "Мама", 0, 1, "'мама'" );
	assertMatches( "Мама", 0, 1, "'ма.*а'" );
	assertMatches( "Маша", 0, 1, "'ма.*а'" );
	assertMatches( "Мамаша", 0, 1, "'ма.*а'" );
	assertNoMatches( "Мошка", "'ма.*а'" );
}

static void testSimplePatterns() {
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
}

static void testTerm() {
	// Term
	assertMatches( "Процессор базы данных", 0, 3, "\"процессор\" \"базы\" \"данных\"" );
	assertMatches( "Процессор базы данных", 0, 3, "N1<процессор> { \"базы\" \"данных\" | \"ввода-вывода\" }<1,1>" );
}

static void testLoopRestrictions() {
	// Loop restrictions: positive
	assertMatches( "Мама мыла раму", 0, 2, "Act = {N} V" );
	assertMatches( "Мама мыла раму", 0, 2, "Act = {N} V<N=V>" );
	assertMatches( "Мама мыла раму", 0, 2, "Act = {N} V<N.g=V.g>" );

	// Loop restriction: empty loop
	assertMatches( "Мама мыла раму", 0, 1, "Act = [A] N" );
	assertMatches( "Мама мыла раму", 0, 1, "Act = {A} N" );
	assertMatches( "Мама мыла раму", 0, 1, "Act = {A} N <A=N>" );

	// Loop restriction: failing longest
	assertMatches( "быстрое белая кошка", 1, 3, "Act = {A} N <A=N>" );
}

static void testLoopAlternatives() {
	assertMatches( "Да да да", 0, 3, "AAA = { 'да' | 'нет' }<3>" );
	assertMatches( "Нет нет нет", 0, 3, "AAA = { 'да' | 'нет' }<3>" );
	assertMatches( "Да нет да", 0, 3, "AAA = { 'да' | 'нет' }<3>" );
}

static void testMultipleEquality() {
	// Multiple equality: positive
	assertMatches( "Белая кошка мыла раму", 0, 3, "Act = A N V<A=N=V>" );
	assertMatches( "Белая кошка мыла раму", 0, 3, "Act = A N V<A.g=N.g=V.g>" );

	// Multiple equality: negative
	assertNoMatches( "Белый кошка мыла раму", "Act = A N V<A=N=V>" );
	assertNoMatches( "Белая кот мыла раму", "Act = A N V<A=N=V>" );
	assertNoMatches( "Белая кошка мыл раму", "Act = A N V<A=N=V>" );
	assertNoMatches( "Белый кошка мыла раму", "Act = A N V<A.g=N.g=V.g>" );
}

static void testBaseEquality() {
	// Base equality
	assertMatches( "Мама, мама, что я буду делать?", 0, 3, "Act = W1 \",\" W2 <W1.b=W2.b>" );

	assertNoMatches( "Кот прыгнул на стол", "Act = N<g=fem> V" );
	assertNoMatches( "Кот прыгнула на стол", "Act = N V<N=V>" );
	assertNoMatches( "Кот прыгнул на стол", "Act = W1 W2 <W1.b=W2.b>" );

	assertMatches( "Мама мыла раму", 0, 2, "Act = N V<N=V> ( N V )" );
}

static void testReusing1() {
	NamespaceRef ns = new Namespace();

	assertMatchesNS( ns, "Белая кошка шла по дорожке", 0, 1, "AA = A (A) | Pa (Pa)" );
	assertMatchesNS( ns, "Белая кошка шла по дорожке", 0, 2, "TestA = AA N" );
	assertMatchesNS( ns, "Белая кошка шла по дорожке", 0, 2, "TestB = AA N <AA=N>" );
}

static void testReusing2() {
	NamespaceRef ns = new Namespace();

	assertNoMatchesNS( ns, "Мама мыла раму", "AA = A (A) | Pa (Pa)" );
	assertMatchesNS( ns, "Мама мыла раму", 0, 1, "TestA = {AA} N" );
	assertMatchesNS( ns, "Мама мыла раму", 0, 1, "TestB = {AA} N <Ap=N>" );
}

static void testCommonNamespace() {
	NamespaceRef ns = new Namespace();

	assertMatchesNS( ns, "Мама мыла раму", 0, 1, "UN = N" );
	assertMatchesNS( ns, "Мама мыла раму", 0, 2, "AB = UN V" );
	assertMatchesNS( ns, "Мама мыла раму", 0, 2, "AC = UN V <UN=V>" );
}

static void testCompoundAttributes() {
	NamespaceRef ns = new Namespace();
	patterns::PatternBuilderRef builder = new patterns::PatternBuilder( ns );

	builder->build( "UV = V (V AS src)" );
	builder->build( "UUV = UV ( UV.src )" );

	assertMatchesNS( ns, "Мама мыла раму", 0, 2, "AA = N UV (UV)" );
	assertMatchesNS( ns, "Мама мыла раму", 0, 2, "AB = N UUV <N=UUV> (UUV)" );
	assertMatchesNS( ns, "Кот мыла раму", 0, 2, "AC = N UUV <N=UUV> (UUV)" ); // TODO Вот такой вот парадокс
}

static void testDictionaries() {
	base::Reference<dictionaries::MemoryDictionary> d = new dictionaries::MemoryDictionary( "DIC" );
	d->add( "МЫТЬ", "РАМА" );

	NamespaceRef ns = new Namespace();
	ns->addDictionary( d );

	assertMatchesNS( ns, "Мама мыла раму", 1, 3, "AA = W1 W2 <DIC(W1,W2)>" );
	assertNoMatchesNS( ns, "Мама мыла раму", "AB = W1 W2 <DIC(W1 W2)>" );
	assertNoMatchesNS( ns, "Мама мыла окно", "AB = W1 W2 <DIC(W1,W2)>" );
}

static void testDictionariesWithConcat() {
	base::Reference<dictionaries::MemoryDictionary> d = new dictionaries::MemoryDictionary( "DIC" );
	d->add( "МЫТЬ РАМА" );

	NamespaceRef ns = new Namespace();
	ns->addDictionary( d );

	assertMatchesNS( ns, "Мама мыла раму", 1, 3, "AA = W1 W2 <DIC(W1 W2)>" );
	assertNoMatchesNS( ns, "Мама мыла раму", "AB = W1 W2 <DIC(W1,W2)>" );
}

static void testDictionariesWithLiterals() {
	base::Reference<dictionaries::MemoryDictionary> d = new dictionaries::MemoryDictionary( "DIC" );
	d->add( "ОКНО", "РАМА" );

	NamespaceRef ns = new Namespace();
	ns->addDictionary( d );

	assertMatchesNS( ns, "Мама мыла раму", 1, 3, "AA = W1 W2 <DIC('окно',W2)>" );
	assertNoMatchesNS( ns, "Мама мыла раму", "AB = W1 W2 <DIC(W1,W2)>" );
}

cute::suite matchingSuite() {
	cute::suite s;

	s += CUTE(testRegexpTokens);
	s += CUTE(testSimplePatterns);
	s += CUTE(testTerm);
	s += CUTE(testLoopRestrictions);
	s += CUTE(testLoopAlternatives);
	s += CUTE(testMultipleEquality);
	s += CUTE(testBaseEquality);
	s += CUTE(testReusing1);
	s += CUTE(testReusing2);
	s += CUTE(testCommonNamespace);
	s += CUTE(testCompoundAttributes);
	s += CUTE(testDictionaries);
	s += CUTE(testDictionariesWithConcat);
	s += CUTE(testDictionariesWithLiterals);

	return s;
}

} }
