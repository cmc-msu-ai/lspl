
#ifndef _LSPL_PATTERNTRANSFORMPARSER_H_
#define _LSPL_PATTERNTRANSFORMPARSER_H_

//#define BOOST_SPIRIT_DEBUG

#include "../base/BaseInternal.h"

#define PHOENIX_LIMIT 4
#define BOOST_SPIRIT_CLOSURE_LIMIT 4

#include "../utils/RusConsts.h"

#include "PatternBuilder.h"
#include "Pattern.h"

#include "parsers/Functions.h"
#include "parsers/VariableParser.h"
#include "parsers/AttributeKeyParser.h"

#include "../text/attributes/AttributeKey.h"

#include "matchers/Matcher.h"
#include "matchers/TokenMatcher.h"

#include "../Namespace.h"

#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_attribute.hpp>
#include <boost/spirit/include/classic_symbols.hpp>
#include <boost/spirit/include/classic_error_handling.hpp>
#include <boost/spirit/include/classic_utility.hpp>
#include <boost/spirit/include/classic_dynamic.hpp>
#include <boost/spirit/include/phoenix1.hpp>

#include <boost/format.hpp>

#include <iostream>
#include <string>

using namespace std;
using namespace boost::spirit::classic;
using namespace phoenix;

using namespace lspl::text::attributes;

using namespace lspl::patterns::restrictions;
using namespace lspl::patterns::expressions;
using namespace lspl::patterns::matchers;
using namespace lspl::patterns::parsers;

LSPL_REFCOUNT_CLASS( lspl::patterns::PatternTransformParser );

namespace lspl { namespace patterns {








struct transformClosure : boost::spirit::classic::closure< transformClosure, boost::ptr_vector<Matcher>* > {
	member1 matchers;
};

class PatternTransformParser : public grammar<PatternTransformParser, transformClosure::context_t> {
public:

	struct AgreementRestrictionClosure : boost::spirit::classic::closure< AgreementRestrictionClosure, Restriction *, boost::ptr_vector<Expression> > {
		member1 restriction;
		member2 args;
	};

	struct MatcherClosure : boost::spirit::classic::closure< MatcherClosure, uint, boost::ptr_vector<Restriction> > {
		member1 index;
		member2 restrictions;
	};

	struct TokenMatcherClosure : boost::spirit::classic::closure< TokenMatcherClosure, std::string > {
		member1 token;
	};

	struct WordMatcherClosure : boost::spirit::classic::closure< WordMatcherClosure, std::string, SpeechPart > {
		member1 base;
		member2 speechPart;
	};

	struct PatternMatcherClosure : boost::spirit::classic::closure< PatternMatcherClosure, std::string > {
		member1 name;
	};

	struct ExpressionClosure : boost::spirit::classic::closure< ExpressionClosure, Expression*, boost::ptr_vector<Expression> > {
		member1 exp;
		member2 args;
	};
/*
	enum Errors {
		BindingEndMissing,
		RestrictionEndMissing,
		LoopEndMissing,
		OptionalEndMissing,
		NoMatchersInAlternative,
		NoMatchersInGroup,
		NoRestrictionBody,
		InvalidPatternName,
		ClosingSglQuoteMissed,
		ClosingDblQuoteMissed,
		AttributeValueExpected
	};
*/
	template <typename ScannerT> class definition {
    public:
  		definition( const PatternTransformParser & self_c ) : variable( typeSymbol ) {
 	    	PatternTransformParser * self = const_cast<PatternTransformParser *>( &self_c );


/*       	assertion<Errors> expect_restriction_end(RestrictionEndMissing);
        	assertion<Errors> expect_restriction_body(NoRestrictionBody);
          	assertion<Errors> expect_closing_sgl_quote(ClosingSglQuoteMissed);
        	assertion<Errors> expect_closing_dbl_quote(ClosingDblQuoteMissed);
*/
        	function<AddImpl> add;
			function<AddRestrictionImpl> addRestriction;
        	function<AddStringMatcherImpl> addStringMatcher;
        	function<AddWordMatcherImpl> addWordMatcher;
        	function<AddPatternMatcherImpl> addPatternMatcher( AddPatternMatcherImpl( *self->space, typeSymbol ) );
			function<CreateAgreementRestrictionImpl> createAgreementRestriction;
			function<AddNormalizationRestrictionImpl> AddNormalizationRestriction;

			function<CreateCurrentAttributeExpressionImpl> createCurrentAttributeExpression;
			function<CreateVariableExpressionImpl> createVariableExpression;
			function<CreateAttributeExpressionImpl> createAttributeExpression;
			function<CreateConcatExpressionImpl> createConcatExpression;
			function<CreateStringLiteralExpressionImpl> createStringLiteralExpression;
			function<CreateLiteralExpressionImpl> createLiteralExpression;

//       	endRestriction = expect_restriction_end( ch_p('>') );

			/*синтезируемый_шаблон ::= шаблон_синтеза { , шаблон_синтеза}*/
			source = eps_p[ self->matchers = new boost::ptr_vector<Matcher>() ] >> matcher >> *( matcher );

			/*шаблон_синтеза ::= ссылка | строковая_часть */
			matcher = link | tokenMatcher;

	       	matcherVariable = epsilon_p[ matcher.index = 0 ] >> !uint_p[ matcher.index = arg1 ];

			/*ссылка ::= $ имя_элемента_слова | имя_экземпляра_шаблона*/
			link = ch_p('$') >> ( wordMatcher | patternMatcher );

			/*строковая_часть ::= *набор символов, не содержащих знак '$'* */
			tokenMatcher = lexeme_d[ (+~ch_p('$'))[tokenMatcher.token = construct_<std::string>( arg1, arg2 )] ]
					[addStringMatcher( *(self->matchers), tokenMatcher.token )];

        	/*
        	 * Парсер сопоставителя лексем
        	 */
//        	tokenMatcher = lexeme_d[ switch_p[
//        	         case_p< '"' >( (+~ch_p('"'))[ tokenMatcher.token = construct_<std::string>( arg1, arg2 ) ] >> /*expect_closing_dbl_quote(*/ ch_p('"') /*)*/ ),
//        	         case_p< '\'' >( (+~ch_p('\''))[ tokenMatcher.token = construct_<std::string>( arg1, arg2 ) ] >> /*expect_closing_sgl_quote(*/ ch_p('\'') /*)*/ )
//        	    ] ][ addTokenMatcher( *(self->matchers), tokenMatcher.token ) ];

        	/*
        	 * Парсер сопоставителя слов
        	 */
//			wordMatcherName = ( wordTypeName >> matcherVariable )[ addWordMatcher( *(self->matchers), wordMatcherName.base, wordMatcherName.speechPart, matcher.index, matcher.restrictions ) ];

//         	wordTypeName = lexeme_d[ speechPart[ wordMatcherName.speechPart = arg1 ] >> ~epsilon_p(chset_p("a-zA-Z")) ];

	       	wordMatcher = ( wordType >> matcherVariable >> !wordRestriction )
        		[ addWordMatcher( *(self->matchers), wordMatcher.base, wordMatcher.speechPart, matcher.index, matcher.restrictions ) ];

        	wordType = lexeme_d[ speechPart[ wordMatcher.speechPart = arg1 ] >> ~epsilon_p(chset_p("a-zA-Z")) ];

        	wordRestriction = ch_p('.')[ wordMatcher.base = "" ]
        	    >> (
//        	    		( wordBase >> !(chset_p(";,") >> ( matcherRestriction[add( matcher.restrictions, arg1 )] % ',' )) ) |
        	    		( matcherRestriction[add( matcher.restrictions, arg1 )] /*% ','*/ )
        	    );

//        	wordBase = ( lexeme_d[ +chset_p("a-zA-Z0-9" RUS_ALPHA "-") ] >> epsilon_p(chset_p(";,>")) )[ wordMatcher.base = construct_<std::string>( arg1, arg2 ) ];

        	/*
        	 * Парсер сопоставителя шаблонов
        	 */
//			patternMatcherName = ( patternName[ patternMatcherName.name = construct_<std::string>( arg1, arg2 ) ] >> matcherVariable )[ addPatternMatcher( *(self->matchers), patternMatcherName.name, matcher.index, matcher.restrictions ) ];

        	patternMatcher = ( patternName[ patternMatcher.name = construct_<std::string>( arg1, arg2 ) ] >> matcherVariable >> !( '.' >> ( matcherRestriction[add( matcher.restrictions, arg1 )] /*% ','*/ ) /*>> '>'*/ ) )[ addPatternMatcher( *(self->matchers), patternMatcher.name, matcher.index, matcher.restrictions ) ];

        	patternName = lexeme_d[ +chset_p("a-zA-Z" RUS_ALPHA "-") >> ~epsilon_p(chset_p("a-zA-Z" RUS_ALPHA "-")) ];

        	/*
        	 * Парсеры ограничений
        	 */
        	matcherRestriction = ( localExpression[ add( matcherRestriction.args, arg1 ) ] /*>> *( '=' >> expression[ add( matcherRestriction.args, arg1 ) ] )*/ )
				[ matcherRestriction.restriction = createAgreementRestriction( matcherRestriction.args ) ];

        	patternRestrictions = '<' >>  ( agreementRestriction[addRestriction( *(self->matchers), arg1 )]	) % ',' >> ch_p('>');

	       	agreementRestriction = ( expression[ add( agreementRestriction.args, arg1 ) ] % str_p("~>") )
        		[ agreementRestriction.restriction = createAgreementRestriction( agreementRestriction.args ) ];

        	/*
        	 * Парсеры выражений
        	 */
        	expression = ~eps_p( str_p( "AS" ) ) >> ( stringLiteralExpression | propertyExpression | literalExpression ) >>
            	*( expression[ expression.exp = createConcatExpression( expression.exp, arg1 ) ] );

        	localExpression = attributeKey[ localExpression.exp = createCurrentAttributeExpression( arg1 ) ];

        	propertyExpression = variable[ expression.exp = createVariableExpression( arg1 ) ] >>
    			*( '.' >> attributeKey[ expression.exp = createAttributeExpression( expression.exp, arg1 ) ] );

        	stringLiteralExpression = lexeme_d[ switch_p[
        	    case_p< '"' >( (+~ch_p('"'))[ expression.exp = createStringLiteralExpression( arg1, arg2 ) ] >> /*expect_closing_dbl_quote(*/ ch_p('"') /*)*/ ),
        	    case_p< '\'' >( (+~ch_p('\''))[ expression.exp = createStringLiteralExpression( arg1, arg2 ) ] >> /*expect_closing_sgl_quote(*/ ch_p('\'') /*)*/ )
        	] ];

        	literalExpression = attributeValue[ expression.exp = createLiteralExpression( arg1 ) ];

        	/*
        	 * Таблицы символов
        	 */

        	for ( uint i = 0; i < SpeechPart::COUNT; ++ i ) {
        		speechPart.add( SpeechPart( i ).getAbbrevation().c_str(), SpeechPart( i ) );
        		typeSymbol.add( SpeechPart( i ).getAbbrevation().c_str(), i );
        	}

        	for ( uint i = 0; i < AttributeValue::indexedCount(); ++ i ) {
        		attributeValue.add( AttributeValue( i ).getAbbrevation().c_str(), AttributeValue( i ) );
        	}

/*        	BOOST_SPIRIT_DEBUG_RULE(source);
        	BOOST_SPIRIT_DEBUG_RULE(matcher);
        	BOOST_SPIRIT_DEBUG_RULE(link);
        	BOOST_SPIRIT_DEBUG_RULE(patternString);
        	BOOST_SPIRIT_DEBUG_RULE(wordMatcher);
        	BOOST_SPIRIT_DEBUG_RULE(patternMatcher);
*/
		}

		rule<ScannerT> const & start() const { return source; }

	private:
	   	symbols<uint> typeSymbol;
    	symbols<SpeechPart> speechPart;
    	symbols<AttributeValue> attributeValue;

    	rule<ScannerT> patternName, wordType, source, wordRestriction, matcherVariable, patternRestrictions, endRestriction, link;
    	rule<ScannerT, MatcherClosure::context_t> matcher;
	   	rule<ScannerT, TokenMatcherClosure::context_t> tokenMatcher, patternString;
    	rule<ScannerT, WordMatcherClosure::context_t> wordMatcher;
    	rule<ScannerT, PatternMatcherClosure::context_t> patternMatcher;
    	rule<ScannerT, AgreementRestrictionClosure::context_t> agreementRestriction, matcherRestriction;

    	rule<ScannerT, ExpressionClosure::context_t> expression, localExpression;
    	rule<ScannerT> propertyExpression, stringLiteralExpression, literalExpression;

    	VariableParser variable;
    	AttributeKeyParser attributeKey;

	};

    PatternTransformParser(NamespaceRef space) : space(space) {}
    ~PatternTransformParser() {}

	lspl::NamespaceRef space;

/*    PatternBuilder::BuildInfo build( const char * str ) throw (PatternBuildingException) {
    	try {
		parse_info<const char *> pi = boost::spirit::classic::parse( str, *this, space_p );

        	PatternBuilder::BuildInfo bi;
        	bi.parseLength = (uint) pi.length;
        	bi.parseTail = pi.stop;

        	return bi;
    	} catch ( parser_error<Errors,const char *> & err ) {
    		switch( err.descriptor ) {
    		case BindingEndMissing:
    			throw PatternBuildingException( "Binding end missing" );
    		case RestrictionEndMissing:
    			throw PatternBuildingException( "Restriction end missing" );
    		case LoopEndMissing:
    		    throw PatternBuildingException( "Loop end missing" );
    		case OptionalEndMissing:
    		    throw PatternBuildingException( "Optional group end missing" );
    		case NoMatchersInAlternative:
    		    throw PatternBuildingException( "No matchers in alternative" );
    		case NoMatchersInGroup:
    		    throw PatternBuildingException( "No matchers in group" );
    		case NoRestrictionBody:
    		    throw PatternBuildingException( "No valid restrictions" );
    		case InvalidPatternName:
    		    throw PatternBuildingException( "Invalid pattern name" );
    		case ClosingSglQuoteMissed:
    			throw PatternBuildingException( "Closing single quote missed" );
    		case ClosingDblQuoteMissed:
    			throw PatternBuildingException( "Closing double quote missed" );
    		case AttributeValueExpected:
    			throw PatternBuildingException( "Invalid or no attribute value" );
    		default:
        		throw PatternBuildingException( (boost::format( "Error parsing template: %1%. Descriptor: %2%. Where: %3%" ) % err.what() % err.descriptor % err.where).str() );
    		}
    	} catch ( const std::exception & e ) {
    		throw PatternBuildingException( (boost::format( "Error parsing template: %1%" ) % e.what()).str() );
    	} catch ( ... ) {
    		throw PatternBuildingException( "Unknown error during parsing template" );
    	}
    }
*/
};






} } // namespace lspl::patterns

#endif /*_LSPL_PATTERNTRANSFORMPARSER_H_*/
