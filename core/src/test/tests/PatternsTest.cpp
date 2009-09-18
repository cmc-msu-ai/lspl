/*
 * PatternsTest.cpp
 *
 *  Created on: Sep 27, 2008
 *      Author: alno
 */

#include "Tests.h"

#include "../assertions/PatternsAssertions.h"

#include <iostream>

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

void testPatterns() {
	assert( Matcher::aliveObjectsCount == 0 );
	assert( Restriction::aliveObjectsCount == 0 );

	std::cout << "Testing pattern building..." << std::endl;

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
	assertBuilds( "AAA = A N { V }<1> <N=V>" );
	assertBuilds( "AAA = A N { V } <N=V>" );
	assertBuilds( "AAA = A N { V } <A=N=V>" );

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

void testPatternStructure() {
	std::cout << "Testing equality..." << std::endl;

	Pattern p1("TP");
	Alternative & a = p1.newAlternative( "A N" );
	a.newWordMatcher( "",SpeechPart::ADJECTIVE );
	a.newWordMatcher( "",SpeechPart::NOUN );

	{
		Alternative & a2 = p1.newAlternative( "A N" );
		a2.newWordMatcher( "",SpeechPart::ADJECTIVE );
		a2.newWordMatcher( "",SpeechPart::NOUN );

		assertTrue( a.equals( a2 ), "a1 should be equal a2" );

		Alternative & a3 = p1.newAlternative( "A A" );
		a3.newWordMatcher( "",SpeechPart::ADJECTIVE );
		a3.newWordMatcher( "",SpeechPart::ADJECTIVE );

		assertFalse( a.equals( a3 ), "a1 shouldn't be equal a3" );

		Alternative & a4 = p1.newAlternative( "A N A" );
		a4.newWordMatcher( "",SpeechPart::ADJECTIVE );
		a4.newWordMatcher( "",SpeechPart::NOUN );
		a4.newWordMatcher( "",SpeechPart::ADJECTIVE );

		assertFalse( a.equals( a4 ), "a1 shouldn't be equal a4" );

		Alternative & a5 = p1.newAlternative( "A { N }" );
		a4.newWordMatcher( "",SpeechPart::ADJECTIVE );
		a4.newLoopMatcher(0,0).newAlternative().newWordMatcher( "",SpeechPart::NOUN );

		assertFalse( a.equals( a5 ), "a1 shouldn't be equal a5" );
	}

	// With restrictions
	{
		AgreementRestriction * r6 = new AgreementRestriction();
		r6->addArgument( new AttributeExpression( new CurrentAnnotationExpression(), AttributeKey::CASE ) );
		r6->addArgument( new ConstantExpression( AttributeValue::ACCUSATIVE ) );

		Alternative & a6 = p1.newAlternative( "A<c=acc> N" );
		a6.newWordMatcher( "",SpeechPart::ADJECTIVE ).addRestriction( r6 );
		a6.newWordMatcher( "",SpeechPart::NOUN );

		assertFalse( a.equals( a6 ), "a1 shouldn't be equal a6" );

		AgreementRestriction * r61 = new AgreementRestriction();
		r61->addArgument( new AttributeExpression( new CurrentAnnotationExpression(), AttributeKey::CASE ) );
		r61->addArgument( new ConstantExpression( AttributeValue::ACCUSATIVE ) );

		Alternative & a61 = p1.newAlternative( "A<c=acc> N" );
		a61.newWordMatcher( "",SpeechPart::ADJECTIVE ).addRestriction( r61 );
		a61.newWordMatcher( "",SpeechPart::NOUN );

		assertTrue( a6.equals( a61 ), "a6 should be equal a61" );

		AgreementRestriction * r7 = new AgreementRestriction();
		r7->addArgument( new AttributeExpression( new CurrentAnnotationExpression(), AttributeKey::CASE ) );
		r7->addArgument( new ConstantExpression( AttributeValue::NOMINATIVE ) );

		Alternative & a7 = p1.newAlternative( "A<c=acc> N" );
		a7.newWordMatcher( "",SpeechPart::ADJECTIVE ).addRestriction( r7 );
		a7.newWordMatcher( "",SpeechPart::NOUN );

		assertFalse( a6.equals( a7 ), "a6 shouldn't be equal a7" );
	}

	// With bindings
	{
		Alternative & ab = p1.newAlternative( "A<c=acc> N" );
		ab.newWordMatcher( "",SpeechPart::ADJECTIVE );
		ab.newWordMatcher( "",SpeechPart::NOUN );
		ab.addBinding( AttributeKey::CASE, new AttributeExpression( new VariableExpression( Variable( SpeechPart::ADJECTIVE, 0 ) ), AttributeKey::CASE ) );

		assertFalse( a.equals( ab ), "a shouldn't be equal ab" );

		Alternative & ab1 = p1.newAlternative( "A<c=acc> N" );
		ab1.newWordMatcher( "",SpeechPart::ADJECTIVE );
		ab1.newWordMatcher( "",SpeechPart::NOUN );
		ab1.addBinding( AttributeKey::CASE, new AttributeExpression( new VariableExpression( Variable( SpeechPart::ADJECTIVE, 0 ) ), AttributeKey::CASE ) );

		assertTrue( ab.equals( ab1 ), "ab should be equal ab1" );

		Alternative & ab2 = p1.newAlternative( "A<c=acc> N" );
		ab2.newWordMatcher( "",SpeechPart::ADJECTIVE );
		ab2.newWordMatcher( "",SpeechPart::NOUN );
		ab2.addBinding( AttributeKey::CASE, new AttributeExpression( new VariableExpression( Variable( SpeechPart::ADJECTIVE, 2 ) ), AttributeKey::CASE ) );

		assertFalse( ab.equals( ab2 ), "ab shouldn't be equal ab2" );

		Alternative & ab3 = p1.newAlternative( "A<c=acc> N" );
		ab3.newWordMatcher( "",SpeechPart::ADJECTIVE );
		ab3.newWordMatcher( "",SpeechPart::NOUN );
		ab3.addBinding( AttributeKey::DOC, new AttributeExpression( new VariableExpression( Variable( SpeechPart::ADJECTIVE, 0 ) ), AttributeKey::CASE ) );

		assertFalse( ab.equals( ab3 ), "ab shouldn't be equal ab3" );
	}

	// Remove duplicate test
	{
		Pattern p2("TP");
		Alternative & a1 = p2.newAlternative( "A N" );
		a1.newWordMatcher( "",SpeechPart::ADJECTIVE );
		a1.newWordMatcher( "",SpeechPart::NOUN );

		Alternative & a2 = p2.newAlternative( "A N" );
		a2.newWordMatcher( "",SpeechPart::ADJECTIVE );
		a2.newWordMatcher( "",SpeechPart::NOUN );

		Alternative & a3 = p2.newAlternative( "A" );
		a3.newWordMatcher( "",SpeechPart::ADJECTIVE );

		p2.removeDuplicateAlternatives();

		assertEquals( 2, p2.alternatives.size() );
		assertEquals( &p2.alternatives[0], &a1 );
		assertEquals( &p2.alternatives[1], &a3 );
	}
}

} }
