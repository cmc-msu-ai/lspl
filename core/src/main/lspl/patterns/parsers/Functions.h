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

#include <boost/spirit/symbols.hpp>
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

struct AddLoopMatcherImpl {

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	struct result { typedef void type; };

	void operator()( boost::ptr_vector<Matcher> & matchers, uint min, uint max, std::vector<uint> & alternativesCount ) const ;
};

struct DefinePattern {
public:
	DefinePattern( Namespace & space, boost::spirit::symbols<uint> & typeSymbol ) :
		space( space ), typeSymbol( typeSymbol ) {}

	PatternRef getPattern( const std::string & name ) const;
private:
	Namespace & space;
	boost::spirit::symbols<uint> & typeSymbol;
};

struct AddPatternMatcherImpl : public DefinePattern {

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	struct result { typedef void type; };

	AddPatternMatcherImpl( Namespace & space, boost::spirit::symbols<uint> & typeSymbol ) :
		DefinePattern( space, typeSymbol ) {}

	void operator()( boost::ptr_vector<Matcher> & matchers, const std::string & name, uint index, boost::ptr_vector< Restriction > & restrictions ) const;
};

struct AddAlternativeDefinitionImpl {

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	struct result { typedef void type; };

	AddAlternativeDefinitionImpl( transforms::TransformBuilder & transformBuilder ) :
		transformBuilder( transformBuilder ) {}

	void operator()( boost::ptr_vector<Alternative> & alts, boost::ptr_vector<Matcher> & matchers, boost::ptr_map<AttributeKey,Expression> & bindings, const std::string & source, const std::string & transformSource ) const;

private:
	transforms::TransformBuilder & transformBuilder;
};

struct AddPatternDefinitionImpl : public DefinePattern {

	template <typename Arg1, typename Arg2>
	   struct result { typedef void type; };

	AddPatternDefinitionImpl( Namespace & space, boost::spirit::symbols<uint> & typeSymbol, transforms::TransformBuilder & transformBuilder ) :
		DefinePattern( space, typeSymbol ), transformBuilder( transformBuilder ) {}

	void operator()( const std::string & name, boost::ptr_vector<Alternative> & alts ) const;

private:
	transforms::TransformBuilder & transformBuilder;
};

struct AddImpl {
	template <typename Arg1, typename Arg2>
	struct result { typedef void type; };

	template <typename Arg1, typename Arg2>
	void operator()( Arg1 & vec, Arg2 value ) const {
		vec.push_back( value );
	}
};

struct AddMatcherRestrictionImpl {
	template <typename Arg1, typename Arg2, typename Arg3>
	struct result { typedef void type; };

	void operator()( boost::ptr_vector<Restriction> & restrictions, AttributeKey type, AttributeValue value ) const;
};

struct AddPatternRestrictionImpl {
	template <typename Arg1, typename Arg2>
	struct result { typedef void type; };

	void operator()( boost::ptr_vector<Matcher> & matchers, const std::vector<std::pair<Variable,AttributeKey> > & elems ) const;
};

struct AddDictionaryRestrictionImpl {
	template <typename Arg1, typename Arg2, typename Arg3>
	struct result { typedef void type; };

	AddDictionaryRestrictionImpl( Namespace & ns ) :
		ns( ns ) {
	}

	void operator()( boost::ptr_vector<Matcher> & matchers, const std::string & dictionaryName, const std::vector<Variable> & variables ) const;

	Matcher & findLastMatcher( boost::ptr_vector<Matcher> & matchers, const std::vector<Variable> & variables ) const;

	Namespace & ns;
};

struct AddBindingImpl {
	template <typename Arg1, typename Arg2, typename Arg3>
	struct result { typedef void type; };

	void operator()( boost::ptr_map<AttributeKey,Expression> & bindings, AttributeKey att, Expression * exp ) const;
};

struct CreateAttributeExpression {
	template <typename Arg1, typename Arg2>
	struct result { typedef Expression * type; };

	Expression * operator()( Expression * exp, AttributeKey key ) const;
};

struct CreateVariableExpression {
	template <typename Arg1>
	struct result { typedef Expression * type; };

	Expression * operator()( Variable var ) const;
};

struct CreateConcatExpression {
	template <typename Arg1, typename Arg2>
	struct result { typedef Expression * type; };

	Expression * operator()( Expression * exp1, Expression * exp2 ) const;
};

} } }

#endif /* _LSPL_PATTERNS_PARSERS_FUNCTIONS_H_ */
