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
#include <lspl/patterns/restrictions/AgreementRestriction.h>
#include <lspl/patterns/expressions/VariableExpression.h>
#include <lspl/patterns/expressions/AttributeExpression.h>
#include <lspl/patterns/expressions/ConstantExpression.h>
#include <lspl/text/Text.h>
#include <lspl/text/readers/PlainTextReader.h>
#include <lspl/dictionaries/MemoryDictionary.h>

using namespace lspl::assertions;
using namespace lspl::patterns::expressions;
using namespace lspl::patterns::restrictions;
using namespace lspl::patterns::matchers;
using namespace lspl::text::attributes;

namespace lspl { namespace tests {

static void testRegexpTokens() {
	// Regexp tokens
	assertMatches( "����", 0, 1, "Act = \"����\"" );
	assertMatches( "����", 0, 1, "Act = \"��.*�\"" );
	assertMatches( "����", 0, 1, "Act = \"��.*�\"" );
	assertMatches( "������", 0, 1, "Act = \"��.*�\"" );
	assertNoMatches( "�����", "Act = \"��.*�\"" );
}

static void testSimplePatterns() {
	// Simple
	assertMatches( "���� ���� ����", 0, 2, "Act = N V" );
	assertMatches( "���� ���� ����", 0, 2, "Act = N<����> V" );
	assertMatches( "���� ���� ����", 0, 2, "Act = N V<<N=V>>" );
	assertMatches( "���� ���� ����", 0, 2, "Act = N { V<<N=V>> }<1,1>" );
	assertMatches( "���� ���� ����", 0, 2, "Act = N { { V<<N=V>> }<1,1> }<1,1>" );
	assertMatches( "���� ���� ����", 0, 2, "Act = N V<<N.g=V.g>>" );
	assertMatches( "���� ���� ����", 0, 2, "Act = N<g=fem> V" );
	assertMatches( "���� ���� ����", 0, 2, "Act = \"����\" V" );
	assertNoMatches( "���� ���� ����", "Act = N<����> V" );
}

static void testRestrictionsOnBase() {
	assertMatches( "���� ���� ����", 0, 2, "Act = N V <lemma=����>" );
	assertMatches( "���� ���� ����", 0, 2, "Act = N V <����>" );
	assertMatches( "���� ���� ����", 1, 2, "Act = V <!=������>");
	assertMatches( "���� ���� ����", 1, 2, "Act = V <\".*�.*\">");
	assertNoMatches( "���� ���� ����", "Act = V <\".*�.*\">");
	assertNoMatches( "���� ���� ����", "Act = V <!=����>" );
	assertNoMatches( "���� ���� ����", "Act = N V <lemma=������>" );
	assertNoMatches( "���� ���� ����", "Act = N V<������>" );
}

static void testRestrictionsOnStem() {
	assertMatches( "���� ���� ����", 0, 2, "Act = N V <stem=�>" );
	assertMatches( "���� ���� ����", 0, 2, "Act = N V <stem=\".\">" );
	assertNoMatches( "���� ���� ����", "Act = N V <stem=���>" );
	assertMatches( "���� ���� ����", 1, 2, "Act = V <stem!=���>" );
	assertNoMatches( "���� ���� ����", "Act = N V <stem=���>" );
}

static void testTerm() {
	// Term
	assertMatches( "��������� ���� ������", 0, 3, "Act = \"���������\" \"����\" \"������\"" );
	assertMatches( "��������� ���� ������", 0, 3, "Act = \"��������� ���� ������\"" );
	assertMatches( "��������� ���� ������", 0, 3, "Act = N1<���������> { \"����\" \"������\" | \"�����-������\" }<1,1>" );
}

static void testTokensInLoop() {
	assertMatches( "���� ���� ����", 0, 1, "Act = { \"����\" }<1>" );
	assertMatches( "���� ���� ����", 0, 2, "Act = { \"����\" }<2>" );
	assertMatches( "�� �� ��", 0, 3, "AAA = { \"��\" }<3>" );
}

static void testLoopRestrictions() {
	// Loop restrictions: positive
	assertMatches( "���� ���� ����", 0, 2, "Act = {N} V" );
	assertMatches( "���� ���� ����", 0, 2, "Act = {N} V<<N=V>>" );
	assertMatches( "���� ���� ����", 0, 2, "Act = {N} V<<N.g=V.g>>" );

	// Loop restriction: empty loop
	assertMatches( "���� ���� ����", 0, 1, "Act = [A] N" );
	assertMatches( "���� ���� ����", 0, 1, "Act = {A} N" );
	assertMatches( "���� ���� ����", 0, 1, "Act = {A} N <<A=N>>" );

	// Loop restriction: failing longest
	assertMatches( "������� ����� �����", 1, 3, "Act = {A} N <<A=N>>" );
}

static void testLoopAlternatives() {
	assertMatches( "�� �� ��", 0, 3, "AAA = { \"��\" | \"���\" }<3>" );
	assertMatches( "��� ��� ���", 0, 3, "AAA = { \"��\" | \"���\" }<3>" );
	assertMatches( "�� ��� ��", 0, 3, "AAA = { \"��\" | \"���\" }<3>" );
}

static void testMultipleEquality() {
	// Multiple equality: positive
	assertMatches( "����� ����� ���� ����", 0, 3, "Act = A N V<<A=N=V>>" );
	assertMatches( "����� ����� ���� ����", 0, 3, "Act = A N V<<A.g=N.g=V.g>>" );

	// Multiple equality: negative
	assertNoMatches( "����� ����� ���� ����", "Act = A N V<<A=N=V>>" );
	assertNoMatches( "����� ��� ���� ����", "Act = A N V<<A=N=V>>" );
	assertNoMatches( "����� ����� ��� ����", "Act = A N V<<A=N=V>>" );
	assertNoMatches( "����� ����� ���� ����", "Act = A N V<<A.g=N.g=V.g>>" );
}

static void testBaseEquality() {
	// Base equality
	assertMatches( "����, ����, ��� � ���� ������?", 0, 3, "Act = W1 \",\" W2 <<W1.b=W2.b>>" );

	assertNoMatches( "��� ������� �� ����", "Act = N<g=fem> V" );
	assertNoMatches( "��� �������� �� ����", "Act = N V<<N=V>>" );
	assertNoMatches( "��� ������� �� ����", "Act = W1 W2 <<W1.b=W2.b>>" );

	assertMatches( "���� ���� ����", 0, 2, "Act = N V<<N=V>> " );
}

static void testReusing1() {
	NamespaceRef ns = new Namespace();

	assertMatchesNS( ns, "����� ����� ��� �� �������", 0, 1, "AA (A, Pa) = A | Pa" );
	assertMatchesNS( ns, "����� ����� ��� �� �������", 0, 2, "TestA = AA N" );
	assertMatchesNS( ns, "����� ����� ��� �� �������", 0, 2, "TestB = AA N <<AA=N>>" );
}

static void testReusing2() {
	NamespaceRef ns = new Namespace();

	assertNoMatchesNS( ns, "���� ���� ����", "AA(A, Pa) = A | Pa" );
	assertMatchesNS( ns, "���� ���� ����", 0, 1, "TestA = {AA} N" );
	assertMatchesNS( ns, "���� ���� ����", 0, 1, "TestB = {AA} N <<Ap=N>>" );
}

static void testCommonNamespace() {
	NamespaceRef ns = new Namespace();

	assertMatchesNS( ns, "���� ���� ����", 0, 1, "UN = N" );
	assertMatchesNS( ns, "���� ���� ����", 0, 2, "AB = UN V" );
	assertMatchesNS( ns, "���� ���� ����", 0, 2, "AC = UN V <<UN=V>>" );
}

static void testStrictAgreement() {
	assertMatches( "��� ���", 0, 2, "Pattern = W1 W2 <<W1=W2>>" );
	assertNoMatches( "��� ���", "Pattern = W1 W2 <<W1==W2>>" );
	assertMatches( "��� ���", 0, 2, "Pattern = W1 W2 <<W1==W2>>" );
}

/*
static void testCompoundAttributes() {
	NamespaceRef ns = new Namespace();
	patterns::PatternBuilderRef builder = new patterns::PatternBuilder( ns );

	builder->build( "UV = V (V AS src)" );
	builder->build( "UUV = UV ( UV.src )" );

	assertMatchesNS( ns, "���� ���� ����", 0, 2, "AA = N UV (UV)" );
	assertMatchesNS( ns, "���� ���� ����", 0, 2, "AB = N UUV <N=UUV> (UUV)" );
	assertMatchesNS( ns, "��� ���� ����", 0, 2, "AC = N UUV <N=UUV> (UUV)" ); // TODO ��� ����� ��� ��������
}

static void testDictionaries() {
	base::Reference<dictionaries::MemoryDictionary> d = new dictionaries::MemoryDictionary( "DIC" );
	d->add( "����", "����" );

	NamespaceRef ns = new Namespace();
	ns->addDictionary( d );

	assertMatchesNS( ns, "���� ���� ����", 1, 3, "AA = W1 W2 <DIC(W1,W2)>" );
	assertNoMatchesNS( ns, "���� ���� ����", "AB = W1 W2 <DIC(W1 W2)>" );
	assertNoMatchesNS( ns, "���� ���� ����", "AB = W1 W2 <DIC(W1,W2)>" );
}

static void testDictionariesWithConcat() {
	base::Reference<dictionaries::MemoryDictionary> d = new dictionaries::MemoryDictionary( "DIC" );
	d->add( "���� ����" );

	NamespaceRef ns = new Namespace();
	ns->addDictionary( d );

	assertMatchesNS( ns, "���� ���� ����", 1, 3, "AA = W1 W2 <DIC(W1 W2)>" );
	assertNoMatchesNS( ns, "���� ���� ����", "AB = W1 W2 <DIC(W1,W2)>" );
}

static void testDictionariesWithLiterals() {
	base::Reference<dictionaries::MemoryDictionary> d = new dictionaries::MemoryDictionary( "DIC" );
	d->add( "����", "����" );

	NamespaceRef ns = new Namespace();
	ns->addDictionary( d );

	assertMatchesNS( ns, "���� ���� ����", 1, 3, "AA = W1 W2 <DIC('����',W2)>" );
	assertNoMatchesNS( ns, "���� ���� ����", "AB = W1 W2 <DIC(W1,W2)>" );
}*/

static void testRestrictedMatches() {
	AgreementRestriction r;
	r.addArgument( new AttributeExpression( new VariableExpression( SpeechPart::VERB, 1 ), AttributeKey::BASE ) );
	r.addArgument( new ConstantExpression( "����" ) );

	assertMatches( "���� ���� ����", 0, 2, "Act = N1 V1" );
	assertMatches( "������ ��� ��� ��� �� ����", 1, 3, "Act = N1 V1" );

	assertRestrictedMatches( "���� ���� ����", 0, 2, "Act = N1 V1", r );
	assertNoRestrictedMatches( "������ ��� ��� ��� �� ����", "Act = N1 V1", r );
}

cute::suite matchingSuite() {
	cute::suite s;

	s += CUTE(testRegexpTokens);
	s += CUTE(testSimplePatterns);
	s += CUTE(testRestrictionsOnBase);
	s += CUTE(testRestrictionsOnStem);
	s += CUTE(testTerm);
	s += CUTE(testTokensInLoop);
	s += CUTE(testLoopRestrictions);
	s += CUTE(testLoopAlternatives);
	s += CUTE(testMultipleEquality);
	s += CUTE(testBaseEquality);
	s += CUTE(testReusing1);
	s += CUTE(testReusing2);
	s += CUTE(testCommonNamespace);
	//s += CUTE(testCompoundAttributes);
	//s += CUTE(testDictionaries);
	//s += CUTE(testDictionariesWithConcat);
	//s += CUTE(testDictionariesWithLiterals);
	s += CUTE(testRestrictedMatches);
	s += CUTE(testStrictAgreement);

	return s;
}

} }
