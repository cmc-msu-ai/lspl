/*
 * Functions.h
 *
 *  Created on: Apr 16, 2009
 *      Author: alno
 */

#ifndef _LSPL_PATTERNS_PARSERS_FUNCTIONS_H_
#define _LSPL_PATTERNS_PARSERS_FUNCTIONS_H_

#include "../../Namespace.h"

#include "../matchers/Forward.h"

#include "../../transforms/Forward.h"

#include "../restrictions/Restriction.h"

#include "../expressions/Expression.h"

#include "../../text/attributes/SpeechPart.h"
#include "../../text/attributes/AttributeKey.h"

#include <boost/spirit/include/classic_symbols.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_map.hpp>

#include <functional>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

using namespace lspl::text::attributes;

using namespace lspl::patterns::restrictions;
using namespace lspl::patterns::expressions;
using namespace lspl::patterns::matchers;

namespace lspl { namespace patterns { namespace parsers {

struct AddWordMatcherImpl {

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	struct result { typedef void type; };

	void operator()( boost::ptr_vector<Matcher> & matchers, const std::string & base, SpeechPart speechPart, uint index, boost::ptr_vector< Restriction > & restrictions ) const;
};

struct AddTokenMatcherImpl {

	template <typename Arg1, typename Arg2>
	struct result { typedef void type; };

	void operator()( boost::ptr_vector<Matcher> & matchers, const std::string & token ) const;
};

struct AddTokenMatcherNoRegexpImpl {

	template <typename Arg1, typename Arg2>
	struct result { typedef void type; };

	void operator()( boost::ptr_vector<Matcher> & matchers, const std::string & token ) const;
};

struct AddStringMatcherImpl {

	template <typename Arg1, typename Arg2>
	struct result { typedef void type; };

	void operator()( boost::ptr_vector<Matcher> & matchers, const std::string & token ) const;
};

struct AddLoopMatcherImpl {

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	struct result { typedef void type; };

	void operator()( boost::ptr_vector<Matcher> & matchers, uint min, uint max, std::vector<uint> & alternativesCount ) const ;
};

struct DefinePattern {
public:
	DefinePattern( Namespace & space, boost::spirit::classic::symbols<uint> & typeSymbol ) :
		space( space ), typeSymbol( typeSymbol ) {}

	PatternRef getPattern( const std::string & name ) const;
private:
	Namespace & space;
	boost::spirit::classic::symbols<uint> & typeSymbol;
};

struct AddPatternMatcherImpl : public DefinePattern {

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	struct result { typedef void type; };

	AddPatternMatcherImpl( Namespace & space, boost::spirit::classic::symbols<uint> & typeSymbol ) :
		DefinePattern( space, typeSymbol ) {}

	void operator()( boost::ptr_vector<Matcher> & matchers, const std::string & name, uint index, boost::ptr_vector< Restriction > & restrictions ) const;
};

struct AddAlternativeDefinitionImpl {

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	struct result { typedef void type; };

	AddAlternativeDefinitionImpl( const std::map<std::string, transforms::TransformBuilderRef>& transformBuilders ) :
		transformBuilders( transformBuilders ) {}

	void operator()( boost::ptr_vector<Alternative> & alts, boost::ptr_vector<Matcher> & matchers, boost::ptr_map<AttributeKey,Expression> & bindings, const std::string & source, const std::string & transformSource, const std::string & transformType ) const;

private:
	const std::map<std::string, transforms::TransformBuilderRef>& transformBuilders;
};

struct AddImpl {
	template <typename Arg1, typename Arg2>
	struct result { typedef void type; };

	template <typename Arg1, typename Arg2>
	void operator()( Arg1 & vec, Arg2 value ) const {
		vec.push_back( value );
	}
};

struct AddRestrictionImpl {
	template <typename Arg1,typename Arg2>
	struct result { typedef void type; };

	void operator()( boost::ptr_vector<Matcher> & matchers, Restriction * restriction ) const;

	Matcher & findLastMatcher( boost::ptr_vector<Matcher> & matchers, const Restriction * restriction ) const;
};

struct AddNormalizationRestrictionImpl {
	template <typename Arg1>
	struct result { typedef void type; };

	void operator()( boost::ptr_vector<Restriction> & restrictions ) const;
};

struct AddBindingImpl {
	template <typename Arg1, typename Arg2, typename Arg3>
	struct result { typedef void type; };

	void operator()( boost::ptr_map<AttributeKey,Expression> & bindings, AttributeKey att, Expression * exp ) const;
};

struct CreateDictionaryRestrictionImpl {
	template <typename Arg1, typename Arg2>
	struct result { typedef Restriction * type; };

	CreateDictionaryRestrictionImpl( Namespace & ns ) :
		ns( ns ) {
	}

	Restriction * operator()( const std::string & dictionaryName, boost::ptr_vector<Expression> & args ) const;

	Namespace & ns;
};

struct CreateAgreementRestrictionImpl {
	template <typename Arg1>
	struct result { typedef Restriction * type; };

	Restriction * operator()( boost::ptr_vector<Expression> & args ) const;
};

struct CreateAttributeExpressionImpl {
	template <typename Arg1, typename Arg2>
	struct result { typedef Expression * type; };

	Expression * operator()( Expression * exp, AttributeKey key ) const;
};

struct CreateCurrentAttributeExpressionImpl {
	template <typename Arg1>
	struct result { typedef Expression * type; };

	Expression * operator()( AttributeKey key ) const;
};

struct CreateVariableExpressionImpl {
	template <typename Arg1>
	struct result { typedef Expression * type; };

	Expression * operator()( Variable var ) const;
};

struct CreateConcatExpressionImpl {
	template <typename Arg1, typename Arg2>
	struct result { typedef Expression * type; };

	Expression * operator()( Expression * exp1, Expression * exp2 ) const;
};

struct CreateStringLiteralExpressionImpl {
	template <typename Arg1, typename Arg2>
	struct result { typedef Expression * type; };

	Expression * operator()( const char * start, const char * end ) const;
};

struct CreateLiteralExpressionImpl {
	template <typename Arg1>
	struct result { typedef Expression * type; };

	Expression * operator()( AttributeValue value ) const;
};

} } }

#endif /* _LSPL_PATTERNS_PARSERS_FUNCTIONS_H_ */
