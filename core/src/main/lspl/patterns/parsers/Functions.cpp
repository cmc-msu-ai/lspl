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

static bool isRegexp( const std::string & str ) {
	static std::string regexSymbols( ".[{()\\*+?|^$'" );

	for ( uint i = 0; i < regexSymbols.length(); ++ i )
		if ( str.find( regexSymbols.at(i) ) != std::string::npos )
			return true;

	return false;
}

void AddWordMatcherImpl::operator()( boost::ptr_vector<Matcher> & matchers, const std::string & base, SpeechPart speechPart, uint index, boost::ptr_vector< Restriction > & restrictions ) const {
   	Matcher * matcher = new WordMatcher( base, speechPart );

   	matcher->variable = Variable( speechPart, index );
   	matcher->addRestrictions( restrictions );

   	matchers.push_back( matcher );
}

void AddTokenMatcherImpl::operator()( boost::ptr_vector<Matcher> & matchers, const std::string & token ) const {
	if ( isRegexp( token ) )
		matchers.push_back( new RegexpMatcher( token ) );
	else
		matchers.push_back( new TokenMatcher( token ) );
}

void AddTokenMatcherNoRegexpImpl::operator()( boost::ptr_vector<Matcher> & matchers, const std::string & token ) const {
	matchers.push_back( new TokenMatcher( token ) );
}

void AddStringMatcherImpl::operator()( boost::ptr_vector<Matcher> & matchers, const std::string & token ) const {
	matchers.push_back( new StringMatcher( token ) );
}

void AddLoopMatcherImpl::operator()( boost::ptr_vector<Matcher> & matchers, uint min, uint max, std::vector<uint> & alternativesCount ) const {
	LoopMatcher * matcher = new LoopMatcher( min, max );

	for ( int i = alternativesCount.size() - 1; i >= 0 ; -- i ) { // Важно!! Здесь перебираем в обратном порядке!
		MatcherContainer * matcherGroup = new MatcherContainer(); // Создаем контейнер для альтернативы

		matcherGroup->addMatchers( matchers.end() - alternativesCount[ i ], matchers.end(), matchers );

		matcher->addAlternative( matcherGroup );
	}

	matchers.push_back( matcher );
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

void AddAlternativeDefinitionImpl::operator()( boost::ptr_vector<Alternative> & alts, boost::ptr_vector<Matcher> & matchers, boost::ptr_map<AttributeKey,Expression> & bindings, const std::string & source, const std::string & transformSource ) const {
	Alternative * alternative = new Alternative( source, transformSource ); // Добавляем новую альтернативу к шаблону

	alternative->addMatchers( matchers ); // Добавляем сопоставители
	alternative->addBindings( bindings ); // Добавляем связывания
	alternative->updateDependencies(); // Обновляем зависимости альтернативы

	alts.push_back( alternative );
}

void AddPatternDefinitionImpl::operator()( const std::string & name, boost::ptr_vector<Alternative> & alts ) const {
	PatternRef pattern = getPattern( name );

	pattern->addAlternatives( alts ); // Добавляем альтернативы к шаблону
	pattern->updateDependencies(); // Обновляем зависимости шаблона

	for( boost::ptr_vector<Alternative>::const_iterator altIt = pattern->getAlternatives().begin(); altIt != pattern->getAlternatives().end(); ++ altIt ) {
		const Alternative & alt = *altIt;
		const_cast<Alternative &>( alt ).setTransform( std::auto_ptr<transforms::Transform>( transformBuilder.build( alt, alt.getTransformSource() ) ) ); // Устанавливаем преобразование
	}
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

void AddBindingImpl::operator()( boost::ptr_map<AttributeKey,Expression> & bindings, AttributeKey att, Expression * exp ) const {
	if ( att == AttributeKey::UNDEFINED && dynamic_cast<AttributeExpression*>( exp ) ) {
		bindings.insert( static_cast<AttributeExpression*>( exp )->attribute, exp ); // Если у аттрибута не указано связывание, связываем его с соответствующим аттрибутом
	} else if ( att == AttributeKey::UNDEFINED && dynamic_cast<ConcatenationExpression*>( exp ) ) {
		bindings.insert( att = AttributeKey::TEXT, exp ); // Если у текстового выражения не указано связывание, связываем его с аттрибутом TEXT
	} else {
		bindings.insert( att, exp );
	}
}

Restriction * CreateDictionaryRestrictionImpl::operator()( const std::string & dictionaryName, boost::ptr_vector<Expression> & args ) const {
	dictionaries::DictionaryRef dict = ns.getDictionaryByName( dictionaryName );

	if ( !dict ) // Не нашли словаря - выкидываем исключение
		throw PatternBuildingException( "No dictionary found" );

	DictionaryRestriction * dr = new DictionaryRestriction( dict );

	dr->addArguments( args );

	return dr;
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
