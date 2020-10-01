/*
 * PatternsTest.cpp
 *
 *  Created on: Sep 27, 2008
 *      Author: alno
 */

#include "Tests.h"

#include "../assertions/PatternsAssertions.h"

#include <lspl/patterns/Pattern.h>
#include <lspl/patterns/matchers/Matcher.h>
#include <lspl/patterns/matchers/LoopMatcher.h>
#include <lspl/patterns/matchers/WordMatcher.h>
#include <lspl/patterns/restrictions/Restriction.h>
#include <lspl/patterns/restrictions/AgreementRestriction.h>
#include <lspl/patterns/expressions/AttributeExpression.h>
#include <lspl/patterns/expressions/ConstantExpression.h>
#include <lspl/patterns/expressions/VariableExpression.h>
#include <lspl/patterns/expressions/CurrentAnnotationExpression.h>

#include <lspl/dictionaries/MemoryDictionary.h>

using namespace lspl::assertions;

using lspl::patterns::Alternative;
using lspl::patterns::Pattern;
using lspl::patterns::matchers::Matcher;
using lspl::patterns::matchers::Variable;
using lspl::patterns::restrictions::Restriction;
using lspl::patterns::restrictions::AgreementRestriction;
using lspl::patterns::expressions::AttributeExpression;
using lspl::patterns::expressions::ConstantExpression;
using lspl::patterns::expressions::CurrentAnnotationExpression;
using lspl::patterns::expressions::VariableExpression;

using lspl::text::attributes::SpeechPart;
using lspl::text::attributes::AttributeKey;
using lspl::text::attributes::AttributeValue;

namespace lspl { namespace tests {

static void testSimplePatterns() {
	// Simple
	assertBuilds( "Act = N V" );
	assertBuilds( "Act = N<бизнес> V" );
	assertBuilds( "Act = N<бизнес-вумен> V" );
	assertBuilds( "Act = N<g=fem> V" );
	assertBuilds( "Act = N2<координата, n=plur>" );
	assertBuilds( "Act = V<stem=ход>" );
	assertBuilds( "Act = A N<lemma=подъезд|приезд>" );
	assertBuilds( "Act = N<!=верхний>" );
	assertFails( "Act = N <lemma=один,stem=два>" );
	assertFails( "Act1 = N1 N1" ); // Wrong rule naming: Act1
	assertFails( "Act = N1 N1 <3270 0" );
	assertFails( "Act = N1 N1 <3=7>" );
	assertFails( "N2<координата, n=frdscx>" );
}

static void testPatternNames() {
	// Names
	assertFails( "бизнесвуман = N<бизнес-вумен> V" );
	assertFails( "бизнес-вуман = N<бизнес-вумен> V" );
}

static void testRestrictions() {
	// Restrictions
	assertBuilds( "Act = N V <<N=V>>" );
	assertBuilds( "Act = A N V <<A=N=V>>" );
	assertBuilds( "Act = A N V <<A==N==V>>" );
}

static void testTokens() {
	// Tokens
	assertBuilds( "Act = \"мама\"" );
	assertFails( "Act = 'мама'" );
	assertBuilds( "Act = \"м.*ама\"" );
	assertBuilds( "Act = \"мам+а\"" );

	assertFails( "'мама" );
	assertFails( "\"мама" );
}

static void testLoops() {
	// Loops
	assertBuilds( "AAA = [ N V ]" );
	assertBuilds( "AAA = { N V }" );
	assertBuilds( "AAA = { N V }<1>" );
	assertBuilds( "AAA = { N V }<1,5>" );
	assertBuilds( "AAA = { N V <<N=V>> }<1>" );
	assertBuilds( "AAA = { A N V <<N=V=A>> }<1>" );
	assertBuilds( "AAA = A N { V }<1> <<N=V>>" );
	assertBuilds( "AAA = A N { V } <<N=V>>" );
	assertBuilds( "AAA = A N { V } <<A=N=V>>" );
}

static void testLoopAlternatives() {
	// Alternatives in loops
	assertBuilds( "AAA = [ N | V ]" );
	assertBuilds( "AAA = { N | V }" );
	assertBuilds( "AAA = { N | V }<1>" );
	assertBuilds( "AAA = { N | V }<1,5>" );
	assertBuilds( "AAA = { \"да\" | \"нет\" }" );
}

static void testParameters() {
	// Parameters
	assertBuilds( "AAA ( N ) = N V" );
	assertBuilds( "AAA ( N.c ) = N V" );
	assertBuilds( "AAA ( N.c, V.t ) = N V" );
}

/*static void testConcat() {
	// Concat
	assertBuilds( "AAA = N V ( N V AS f )" );
	assertBuilds( "AAA = N V ( N V )" );
}*/

static void testMultipleEquals() {
	// Multiple equal
	assertBuilds("AAA = N1 N2 N3 <<N1.c=N2.c=N3.c>>");
	assertBuilds("AAA = N1 N2 N3 <<N1=N2=N3>>");
}

/*static void testDictionaries() {
	NamespaceRef ns = new Namespace();
	ns->addDictionary( new dictionaries::MemoryDictionary("DIC") );

	assertBuildsNS( "AAA = N V <DIC(N)>", ns );
	assertBuildsNS( "AAA = N V <DIC(N,V)>", ns );

	assertFailsNS( "AAA = N V <GG(N)>", ns );
}

static void testExpressionsInDictionaries() {
	NamespaceRef ns = new Namespace();
	ns->addDictionary( new dictionaries::MemoryDictionary("DIC") );

	assertBuildsNS( "AAA = N V <DIC(N.c)>", ns );
	assertBuildsNS( "AAA = N V <DIC(V,N.g)>", ns );

	assertBuildsNS( "AAA = N V <DIC(V N)>", ns );
	assertBuildsNS( "AAA = N V <DIC(V N.g)>", ns );
}

static void testLiteralsInDictionaries() {
	NamespaceRef ns = new Namespace();
	ns->addDictionary( new dictionaries::MemoryDictionary("DIC") );

	assertBuildsNS( "AAA = N V <DIC('aaa')>", ns );
	assertBuildsNS( "AAA = N V <DIC(V,'dvf')>", ns );
	assertBuildsNS( "AAA = N V <DIC(V 'gggg')>", ns );
}*/

static void testParentNamespace() {
	NamespaceRef ns1 = new Namespace();

	assertBuildsNS( "NG = A N", ns1 );

	NamespaceRef ns2 = new Namespace( ns1 );

	assertBuildsNS( "AAA = NG V", ns2 );

	assertEquals( ns1->getPatternCount(), 1 );
	assertEquals( ns2->getPatternCount(), 1 );
}

static void testMemoryLeaks() {
	assertTrue( Matcher::aliveObjectsCount == 0 );
	assertTrue( Restriction::aliveObjectsCount == 0 );
}

cute::suite patternBuildingSuite() {
	cute::suite s;

	s += CUTE(testSimplePatterns);
	s += CUTE(testPatternNames);
	s += CUTE(testRestrictions);
	//s += CUTE(testUnnamedPatterns);
	s += CUTE(testTokens);
	s += CUTE(testLoops);
	s += CUTE(testLoopAlternatives);
	s += CUTE(testParameters);
	//s += CUTE(testCompoundAttributes);
	//s += CUTE(testConcat);
	s += CUTE(testMultipleEquals);
	//s += CUTE(testDictionaries);
	//s += CUTE(testExpressionsInDictionaries);
	//s += CUTE(testLiteralsInDictionaries);
	s += CUTE(testParentNamespace);
	s += CUTE(testMemoryLeaks);

	return s;
}

static void testEquality() {
	Pattern p1("TP");
	Alternative & a = p1.newAlternative( "A N" );
	a.newWordMatcher( SpeechPart::ADJECTIVE );
	a.newWordMatcher( SpeechPart::NOUN );

	{
		Alternative & a2 = p1.newAlternative( "A N" );
		a2.newWordMatcher( SpeechPart::ADJECTIVE );
		a2.newWordMatcher( SpeechPart::NOUN );

		assertTrueM( a.equals( a2 ), "a1 should be equal a2" );

		Alternative & a3 = p1.newAlternative( "A A" );
		a3.newWordMatcher( SpeechPart::ADJECTIVE );
		a3.newWordMatcher( SpeechPart::ADJECTIVE );

		assertFalseM( a.equals( a3 ), "a1 shouldn't be equal a3" );

		Alternative & a4 = p1.newAlternative( "A N A" );
		a4.newWordMatcher( SpeechPart::ADJECTIVE );
		a4.newWordMatcher( SpeechPart::NOUN );
		a4.newWordMatcher( SpeechPart::ADJECTIVE );

		assertFalseM( a.equals( a4 ), "a1 shouldn't be equal a4" );

		Alternative & a5 = p1.newAlternative( "A { N }" );
		a4.newWordMatcher( SpeechPart::ADJECTIVE );
		a4.newLoopMatcher(0,0).newAlternative().newWordMatcher( SpeechPart::NOUN );

		assertFalseM( a.equals( a5 ), "a1 shouldn't be equal a5" );
	}

	// With restrictions
	{
		AgreementRestriction * r6 = new AgreementRestriction();
		r6->addArgument( new AttributeExpression( new CurrentAnnotationExpression(), AttributeKey::CASE ) );
		r6->addArgument( new ConstantExpression( AttributeValue::ACCUSATIVE ) );

		Alternative & a6 = p1.newAlternative( "A<c=acc> N" );
		a6.newWordMatcher( SpeechPart::ADJECTIVE ).addRestriction( r6 );
		a6.newWordMatcher( SpeechPart::NOUN );

		assertFalseM( a.equals( a6 ), "a1 shouldn't be equal a6" );

		AgreementRestriction * r61 = new AgreementRestriction();
		r61->addArgument( new AttributeExpression( new CurrentAnnotationExpression(), AttributeKey::CASE ) );
		r61->addArgument( new ConstantExpression( AttributeValue::ACCUSATIVE ) );

		Alternative & a61 = p1.newAlternative( "A<c=acc> N" );
		a61.newWordMatcher( SpeechPart::ADJECTIVE ).addRestriction( r61 );
		a61.newWordMatcher( SpeechPart::NOUN );

		assertTrueM( a6.equals( a61 ), "a6 should be equal a61" );

		AgreementRestriction * r7 = new AgreementRestriction();
		r7->addArgument( new AttributeExpression( new CurrentAnnotationExpression(), AttributeKey::CASE ) );
		r7->addArgument( new ConstantExpression( AttributeValue::NOMINATIVE ) );

		Alternative & a7 = p1.newAlternative( "A<c=acc> N" );
		a7.newWordMatcher( SpeechPart::ADJECTIVE ).addRestriction( r7 );
		a7.newWordMatcher( SpeechPart::NOUN );

		assertFalseM( a6.equals( a7 ), "a6 shouldn't be equal a7" );
	}

	// With bindings
	{
		Alternative & ab = p1.newAlternative( "A<c=acc> N" );
		ab.newWordMatcher( SpeechPart::ADJECTIVE );
		ab.newWordMatcher( SpeechPart::NOUN );
		ab.addBinding( AttributeKey::CASE, new AttributeExpression( new VariableExpression( Variable( SpeechPart::ADJECTIVE, 0 ) ), AttributeKey::CASE ) );

		assertFalseM( a.equals( ab ), "a shouldn't be equal ab" );

		Alternative & ab1 = p1.newAlternative( "A<c=acc> N" );
		ab1.newWordMatcher( SpeechPart::ADJECTIVE );
		ab1.newWordMatcher( SpeechPart::NOUN );
		ab1.addBinding( AttributeKey::CASE, new AttributeExpression( new VariableExpression( Variable( SpeechPart::ADJECTIVE, 0 ) ), AttributeKey::CASE ) );

		assertTrueM( ab.equals( ab1 ), "ab should be equal ab1" );

		Alternative & ab2 = p1.newAlternative( "A<c=acc> N" );
		ab2.newWordMatcher( SpeechPart::ADJECTIVE );
		ab2.newWordMatcher( SpeechPart::NOUN );
		ab2.addBinding( AttributeKey::CASE, new AttributeExpression( new VariableExpression( Variable( SpeechPart::ADJECTIVE, 2 ) ), AttributeKey::CASE ) );

		assertFalseM( ab.equals( ab2 ), "ab shouldn't be equal ab2" );

		Alternative & ab3 = p1.newAlternative( "A<c=acc> N" );
		ab3.newWordMatcher( SpeechPart::ADJECTIVE );
		ab3.newWordMatcher( SpeechPart::NOUN );
		ab3.addBinding( AttributeKey::DOC, new AttributeExpression( new VariableExpression( Variable( SpeechPart::ADJECTIVE, 0 ) ), AttributeKey::CASE ) );

		assertFalseM( ab.equals( ab3 ), "ab shouldn't be equal ab3" );
	}
}

static void testRemoveDuplicates() {
	Pattern p2("TP");
	Alternative & a1 = p2.newAlternative( "A N" );
	a1.newWordMatcher( SpeechPart::ADJECTIVE );
	a1.newWordMatcher( SpeechPart::NOUN );

	Alternative & a2 = p2.newAlternative( "A N" );
	a2.newWordMatcher( SpeechPart::ADJECTIVE );
	a2.newWordMatcher( SpeechPart::NOUN );

	Alternative & a3 = p2.newAlternative( "A" );
	a3.newWordMatcher( SpeechPart::ADJECTIVE );

	p2.removeDuplicateAlternatives();

	assertEquals( 2, p2.getAlternatives().size() );
	assertEquals( p2.getAlternatives()[0].get(), &a1 );
	assertEquals( p2.getAlternatives()[1].get(), &a3 );
}


cute::suite patternStructureSuite() {
	cute::suite s;

	s += CUTE(testEquality);
	s += CUTE(testRemoveDuplicates);

	return s;
}

} }
