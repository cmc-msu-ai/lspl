/*
 * Functions.cpp
 *
 *  Created on: Apr 16, 2009
 *      Author: alno
 */
#include "../../base/BaseInternal.h"

#include "../PatternBuilder.h"

#include "Functions.h"

#include "../Pattern.h"

#include "../matchers/Matcher.h"
#include "../matchers/Variable.h"
#include "../matchers/WordMatcher.h"
#include "../matchers/TokenMatcher.h"
#include "../matchers/RegexpMatcher.h"
#include "../matchers/PatternMatcher.h"
#include "../matchers/LoopMatcher.h"
#include "../matchers/StringMatcher.h"

#include "../expressions/VariableExpression.h"
#include "../expressions/AttributeExpression.h"
#include "../expressions/ConstantExpression.h"
#include "../expressions/ConcatenationExpression.h"
#include "../expressions/CurrentAnnotationExpression.h"

#include "../restrictions/AgreementRestriction.h"
#include "../restrictions/DictionaryRestriction.h"

#include "../../transforms/TransformBuilder.h"

#include "../../morphology/Morphology.h"

#include <iostream>

using namespace lspl::text::attributes;

using namespace lspl::patterns::restrictions;
using namespace lspl::patterns::expressions;
using namespace lspl::patterns::matchers;

using lspl::morphology::Morphology;

namespace lspl { namespace patterns { namespace parsers {

void AddWordMatcherImpl::operator()( boost::ptr_vector<Matcher> & matchers, const std::string & base, SpeechPart speechPart, uint index, boost::ptr_vector< Restriction > & restrictions ) const {
   	WordMatcher * matcher;

   	if (base == "")
   		matcher = new WordMatcher( speechPart );
   	else {
   		LemmaComparator *lcmp = new LemmaComparator(false);
   		lcmp->addAlternativeBase(base);
   		matcher = new WordMatcher( speechPart, lcmp );
   	}

   	matcher->variable = Variable( speechPart, index );
   	matcher->addRestrictions( restrictions );

   	matchers.push_back( matcher );
}

void AddTokenMatcherNoRegexpImpl::operator()( boost::ptr_vector<Matcher> & matchers, const std::string & token ) const {
	matchers.push_back( new TokenMatcher( token ) );
}

void AddStringMatcherImpl::operator()( boost::ptr_vector<Matcher> & matchers, const std::string & token ) const {
	matchers.push_back( new StringMatcher( token ) );
}

PatternRef DefinePattern::getPattern( const std::string & name ) const {
	PatternRef pattern = space.getPatternByName( name );

	if ( !pattern )
		pattern = space.addPattern( new Pattern( name ) );

	typeSymbol.add( name.c_str(), pattern->id + SpeechPart::COUNT );

	return pattern;
}

void AddPatternMatcherImpl::operator()( boost::ptr_vector<Matcher> & matchers, const std::string & name, uint index, boost::ptr_vector< Restriction > & restrictions ) const {
	PatternRef pattern = getPattern( name );

	PatternMatcher * matcher = new PatternMatcher( *pattern );

	matcher->variable = Variable( *pattern, index );
	matcher->addRestrictions( restrictions );

	matchers.push_back( matcher );
}

void AddRestrictionImpl::operator()( boost::ptr_vector<Matcher> & matchers, Restriction * restriction ) const {
	findLastMatcher( matchers, restriction ).addRestriction( restriction );
}

Matcher & AddRestrictionImpl::findLastMatcher( boost::ptr_vector<Matcher> & matchers, const Restriction * restriction ) const {
	if ( restriction->containsCurrentAnnotation() )
		return matchers.back();

	for ( int i = matchers.size() - 1; i >= 0; --i )
		if ( restriction->containsVariable( matchers[i].variable ) )
			return matchers[i];

	return matchers[ 0 ];
}

void AddNormalizationRestrictionImpl::operator()( boost::ptr_vector<Restriction> & restrictions ) const {
	AgreementRestriction *agrRestr = new AgreementRestriction();
	agrRestr->addArgument(new AttributeExpression(new CurrentAnnotationExpression(), lspl::text::attributes::AttributeKey::BASE));
	agrRestr->addArgument(new ConstantExpression("1"));
	Restriction * restriction = agrRestr;
	restrictions.push_back( restriction );
}

Restriction * CreateAgreementRestrictionImpl::operator()( boost::ptr_vector<Expression> & args ) const {
	AgreementRestriction * dr = new AgreementRestriction();

	dr->addArguments( args );

	return dr;
}

Expression * CreateAttributeExpressionImpl::operator()( Expression * exp, AttributeKey key ) const {
	return new AttributeExpression( exp, key );
}

Expression * CreateCurrentAttributeExpressionImpl::operator()( AttributeKey key ) const {
	return new AttributeExpression( new CurrentAnnotationExpression(), key );
}

Expression * CreateVariableExpressionImpl::operator()( Variable var ) const {
	return new VariableExpression( var );
}

Expression * CreateConcatExpressionImpl::operator()( Expression * exp1, Expression * exp2 ) const {
	if ( dynamic_cast<ConcatenationExpression*>( exp2 ) ) { // Справа конкатенация - наращиваем ее слева
		static_cast<ConcatenationExpression*>( exp2 )->args.insert( static_cast<ConcatenationExpression*>( exp2 )->args.begin(), exp1 );
		return exp2;
	} else if ( dynamic_cast<ConcatenationExpression*>( exp1 ) ) { // Слева конкатенация - наращиваем ее справа
		static_cast<ConcatenationExpression*>( exp1 )->addArgument( exp2 );
		return exp1;
	} else { // Создаем новую конкатенацию
		ConcatenationExpression * res = new ConcatenationExpression();
		res->addArgument( exp1 );
		res->addArgument( exp2 );
		return res;
	}
}

Expression * CreateStringLiteralExpressionImpl::operator()( const char * start, const char * end ) const {
	return new ConstantExpression( AttributeValue( Morphology::instance().upcase( start, end ) ) );
}

Expression * CreateLiteralExpressionImpl::operator()( AttributeValue value ) const {
	return new ConstantExpression( value );
}

} } }
