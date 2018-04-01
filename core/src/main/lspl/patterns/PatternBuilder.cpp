#include "../base/BaseInternal.h"

#define PHOENIX_LIMIT 4
#define BOOST_SPIRIT_CLOSURE_LIMIT 4

#include "../utils/RusConsts.h"

#include "PatternBuilder.h"
#include "Pattern.h"

#include "parsers/Functions.h"
#include "parsers/VariableParser.h"
#include "parsers/AttributeKeyParser.h"

#include "matchers/Matcher.h"

#include "../transforms/TransformBuilder.h"
#include "../transforms/TextTransformBuilder.h"
#include "../transforms/PatternTransformBuilder.h"

#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_attribute.hpp>
#include <boost/spirit/include/classic_symbols.hpp>
#include <boost/spirit/include/classic_error_handling.hpp>
#include <boost/spirit/include/classic_utility.hpp>
#include <boost/spirit/include/classic_dynamic.hpp>
#include <boost/spirit/include/phoenix1.hpp>

#include <boost/format.hpp>

using namespace boost::spirit::classic;
using namespace phoenix;

using namespace lspl::text::attributes;

using namespace lspl::patterns::restrictions;
using namespace lspl::patterns::expressions;
using namespace lspl::patterns::matchers;
using namespace lspl::patterns::parsers;

LSPL_REFCOUNT_CLASS( lspl::patterns::PatternBuilder );

namespace lspl { namespace patterns {

class ParserImpl : public grammar<ParserImpl>, public PatternBuilder::Parser {
public:

	struct AgreementRestrictionClosure : public boost::spirit::classic::closure< AgreementRestrictionClosure, Restriction *, boost::ptr_vector<Expression> > {
		member1 restriction;
		member2 args;
	};

	struct DictionaryRestrictionClosure : public boost::spirit::classic::closure< DictionaryRestrictionClosure, Restriction *, std::string, boost::ptr_vector<Expression> > {
		member1 restriction;
		member2 dictionaryName;
		member3 args;
	};

	struct MatcherClosure : public boost::spirit::classic::closure< MatcherClosure, uint, boost::ptr_vector<Restriction> > {
		member1 index;
		member2 restrictions;
	};

	struct WordMatcherClosure : public boost::spirit::classic::closure< WordMatcherClosure, std::string, SpeechPart > {
		member1 base;
		member2 speechPart;
	};

	struct TokenMatcherClosure : public boost::spirit::classic::closure< TokenMatcherClosure, std::string > {
		member1 token;
	};

	struct PatternMatcherClosure : public boost::spirit::classic::closure< PatternMatcherClosure, std::string > {
		member1 name;
	};

	struct LoopMatcherClosure : public boost::spirit::classic::closure< LoopMatcherClosure, uint, uint, std::vector<uint> > {
		member1 min;
		member2 max;
		member3 alternativesCount;
	};

	struct LoopBodyClosure : public boost::spirit::classic::closure< LoopBodyClosure, uint > {
		member1 matcherCount;
	};

	struct PatternClosure : public boost::spirit::classic::closure< PatternClosure, std::string, boost::ptr_vector<Alternative> > {
		member1 name;
		member2 alternatives;
	};

	struct AlternativeClosure : public boost::spirit::classic::closure< AlternativeClosure, uint, boost::ptr_vector<Matcher>, boost::ptr_map<AttributeKey,Expression>, std::string, std::string > {
		member1 stub;
		member2 matchers;
		member3 bindings;
		member4 transformSource;
        member5 transformType;
	};

	struct BindingClosure : public boost::spirit::classic::closure< BindingClosure, AttributeKey, Expression * > {
		member1 att;
		member2 exp;
	};

	struct ExpressionClosure : public boost::spirit::classic::closure< ExpressionClosure, Expression*, boost::ptr_vector<Expression> > {
		member1 exp;
		member2 args;
	};

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

    template <typename ScannerT> class definition {
    public:
    public:
        definition( const ParserImpl & self_c ) : variable( typeSymbol ) {
        	ParserImpl & self = *const_cast<ParserImpl *>( &self_c );

        	assertion<Errors> expect_binding_end(BindingEndMissing);
        	assertion<Errors> expect_restriction_end(RestrictionEndMissing);
        	assertion<Errors> expect_loop_end(LoopEndMissing);
        	assertion<Errors> expect_optional_end(OptionalEndMissing);
        	assertion<Errors> expect_alt_matcher(NoMatchersInAlternative);
        	assertion<Errors> expect_grp_matcher(NoMatchersInGroup);
        	assertion<Errors> expect_restriction_body(NoRestrictionBody);
        	assertion<Errors> expect_valid_pattern_name(InvalidPatternName);
        	assertion<Errors> expect_closing_sgl_quote(ClosingSglQuoteMissed);
        	assertion<Errors> expect_closing_dbl_quote(ClosingDblQuoteMissed);
        	assertion<Errors> expect_attribute_value(AttributeValueExpected);

        	function<AddImpl> add;

			function<AddBindingImpl> addBinding;
			function<AddRestrictionImpl> addRestriction;

        	function<AddPatternMatcherImpl> addPatternMatcher( AddPatternMatcherImpl( *self.space, typeSymbol ) );
        	function<AddWordMatcherImpl> addWordMatcher;
        	function<AddTokenMatcherImpl> addTokenMatcher;
        	function<AddLoopMatcherImpl> addLoopMatcher;
        	function<AddAlternativeDefinitionImpl> addAlternativeDefinition( AddAlternativeDefinitionImpl( self.transformBuilders ) );
        	function<AddPatternDefinitionImpl> addPatternDefinition( AddPatternDefinitionImpl( *self.space, typeSymbol ) );

			function<CreateAgreementRestrictionImpl> createAgreementRestriction;
			function<CreateDictionaryRestrictionImpl> createDictionaryRestriction( *self.space );

			function<CreateCurrentAttributeExpressionImpl> createCurrentAttributeExpression;
			function<CreateVariableExpressionImpl> createVariableExpression;
			function<CreateAttributeExpressionImpl> createAttributeExpression;
			function<CreateConcatExpressionImpl> createConcatExpression;
			function<CreateStringLiteralExpressionImpl> createStringLiteralExpression;
			function<CreateLiteralExpressionImpl> createLiteralExpression;

        	endLoop = expect_loop_end( ch_p( '}' ) );
        	endOptional = expect_optional_end( ch_p( ']' ) );
        	endBinding = expect_binding_end( ch_p(')') );
        	endRestriction = expect_restriction_end( ch_p('>') );

        	source = *pattern;

        	pattern = (
        			( patternName[ pattern.name = construct_<std::string>( arg1, arg2 ) ] >> '=' >> ( alternative % '|' ) ) |
        			( ( alternative % '|' )[ pattern.name = construct_<std::string>( arg1, arg2 ) ] >> !( ch_p('=') >> expect_valid_pattern_name( nothing_p ) ) )
        		)[ addPatternDefinition( pattern.name, pattern.alternatives ) ];

        	alternative = ( matcher >> *(matcher|patternRestrictions) >> !bindingList >> !alternativeTransformSource )
        		[ addAlternativeDefinition( pattern.alternatives, alternative.matchers, alternative.bindings, construct_<std::string>( arg1, arg2 ), alternative.transformSource, alternative.transformType ) ];

        	alternativeTransformSource = ch_p('=') >> lexeme_d[ +chset_p("a-z") ] [ alternative.transformType = construct_<std::string>( arg1, arg2 ) ] >> ch_p('>')
                >> lexeme_d[ *~chset_p("\n|") ][ alternative.transformSource = construct_<std::string>( arg1, arg2 ) ];

        	patternName = lexeme_d[ +chset_p("a-zA-Z" RUS_ALPHA "-") >> ~epsilon_p(chset_p("a-zA-Z" RUS_ALPHA "-")) ];

        	binding = ( expression[ binding.exp = arg1 ] >> !( "AS" >> attributeKey[ binding.att = arg1 ] ) )
        		[ addBinding( alternative.bindings, binding.att, binding.exp ) ];

        	bindingList = '(' >> binding % ',' >> endBinding;

        	/*
        	 * Парсер сопоставителя
        	 */

        	matcher = wordMatcher | tokenMatcher | patternMatcher | loopMatcher;

        	matcherVariable = epsilon_p[ matcher.index = 0 ] >> !uint_p[ matcher.index = arg1 ];

        	/*
        	 * Парсер сопоставителя слов
        	 */

        	wordMatcher = ( wordType >> matcherVariable >> !wordRestriction )
        		[ addWordMatcher( alternative.matchers, wordMatcher.base, wordMatcher.speechPart, matcher.index, matcher.restrictions ) ];

        	wordType = lexeme_d[ speechPart[ wordMatcher.speechPart = arg1 ] >> ~epsilon_p(chset_p("a-zA-Z")) ];

        	wordRestriction = ch_p('<')[ wordMatcher.base = "" ]
        	    >> (
        	    		( wordBase >> !(chset_p(";,") >> ( matcherRestriction[add( matcher.restrictions, arg1 )] % ',' )) ) |
        	    		( matcherRestriction[add( matcher.restrictions, arg1 )] % ',' )
        	    ) >> endRestriction;

        	wordBase = ( lexeme_d[ +chset_p("a-zA-Z0-9" RUS_ALPHA "-") ] >> epsilon_p(chset_p(";,>")) )[ wordMatcher.base = construct_<std::string>( arg1, arg2 ) ];

        	/*
        	 * Парсер сопоставителя лексем
        	 */

        	tokenMatcher = lexeme_d[ switch_p[
        	         case_p< '"' >( (+~ch_p('"'))[ tokenMatcher.token = construct_<std::string>( arg1, arg2 ) ] >> expect_closing_dbl_quote( ch_p('"') ) ),
        	         case_p< '\'' >( (+~ch_p('\''))[ tokenMatcher.token = construct_<std::string>( arg1, arg2 ) ] >> expect_closing_sgl_quote( ch_p('\'') ) )
        	    ] ][ addTokenMatcher( alternative.matchers, tokenMatcher.token ) ];

        	/*
        	 * Парсер сопоставителя шаблонов
        	 */

        	patternMatcher = ( patternName[ patternMatcher.name = construct_<std::string>( arg1, arg2 ) ] >> matcherVariable >> !( '<' >> ( matcherRestriction[add( matcher.restrictions, arg1 )] % ',' ) >> '>' ) )[ addPatternMatcher( alternative.matchers, patternMatcher.name, matcher.index, matcher.restrictions ) ];

        	/*
        	 * Парсер сопоставителя циклов
        	 */
        	loopMatcher = switch_p[
	        		case_p< '{' >( ( loopBody % '|' ) >> endLoop[ loopMatcher.min = 0 ][ loopMatcher.max = 0 ] >> !loopRestriction ),
	        		case_p< '[' >( ( loopBody % '|' ) >> endOptional[ loopMatcher.min = 0 ][ loopMatcher.max = 1 ] )
	        	][ addLoopMatcher( alternative.matchers, loopMatcher.min, loopMatcher.max, loopMatcher.alternativesCount ) ];

        	loopBody = expect_grp_matcher( matcher[ loopBody.matcherCount = 1 ] ) >> ( *(matcher[ loopBody.matcherCount ++ ] | patternRestrictions) )[ add( loopMatcher.alternativesCount, loopBody.matcherCount ) ];

        	loopRestriction = '<' >> epsilon_p(chset_p("0-9,")) >> !uint_p[ loopMatcher.min = arg1 ] >> !( ',' >> uint_p[ loopMatcher.max = arg1 ] ) >> endRestriction;

        	/*
        	 * Парсеры ограничений
        	 */

        	matcherRestriction = ( localExpression[ add( matcherRestriction.args, arg1 ) ] >> *( '=' >> expression[ add( matcherRestriction.args, arg1 ) ] ) )
				[ matcherRestriction.restriction = createAgreementRestriction( matcherRestriction.args ) ];

        	patternRestrictions = '<' >> expect_restriction_body( (
        			agreementRestriction[addRestriction( alternative.matchers, arg1 )] |
        			dictionaryRestriction[addRestriction( alternative.matchers, arg1 )]
				) % ',' ) >> endRestriction;

        	agreementRestriction = ( expression[ add( agreementRestriction.args, arg1 ) ] % '=' )
        		[ agreementRestriction.restriction = createAgreementRestriction( agreementRestriction.args ) ];

        	dictionaryRestriction = ( ( lexeme_d[ +chset_p("a-zA-Z") ][ dictionaryRestriction.dictionaryName = construct_<std::string>( arg1, arg2 ) ] ) >> "(" >> ( expression[ add( dictionaryRestriction.args, arg1 ) ] % "," ) >> ")" )
        		[ dictionaryRestriction.restriction = createDictionaryRestriction( dictionaryRestriction.dictionaryName, dictionaryRestriction.args ) ];

        	/*
        	 * Парсеры выражений
        	 */

        	expression = ~eps_p( str_p( "AS" ) ) >> ( stringLiteralExpression | propertyExpression | literalExpression ) >>
            	*( expression[ expression.exp = createConcatExpression( expression.exp, arg1 ) ] );

        	localExpression = attributeKey[ localExpression.exp = createCurrentAttributeExpression( arg1 ) ];

        	propertyExpression = variable[ expression.exp = createVariableExpression( arg1 ) ] >>
    			*( '.' >> attributeKey[ expression.exp = createAttributeExpression( expression.exp, arg1 ) ] );

        	stringLiteralExpression = lexeme_d[ switch_p[
        	    case_p< '"' >( (+~ch_p('"'))[ expression.exp = createStringLiteralExpression( arg1, arg2 ) ] >> expect_closing_dbl_quote( ch_p('"') ) ),
        	    case_p< '\'' >( (+~ch_p('\''))[ expression.exp = createStringLiteralExpression( arg1, arg2 ) ] >> expect_closing_sgl_quote( ch_p('\'') ) )
        	] ];

        	literalExpression = attributeValue[ expression.exp = createLiteralExpression( arg1 ) ];

        	/*
        	 * Таблицы символов
        	 */

        	for ( uint i = 0; i < SpeechPart::COUNT; ++ i ) {
                speechPart.add( SpeechPart( i ).getAbbrevation().c_str(), SpeechPart( i ) );
                speechPart.add( SpeechPart( i ).getName().c_str(), SpeechPart( i ) );
                typeSymbol.add( SpeechPart( i ).getAbbrevation().c_str(), i );
                typeSymbol.add( SpeechPart( i ).getName().c_str(), i );
        	}

        	for ( uint i = 0; i < AttributeValue::indexedCount(); ++ i ) {
                attributeValue.add( AttributeValue( i ).getAbbrevation().c_str(), AttributeValue( i ) );
                attributeValue.add( AttributeValue( i ).getName().c_str(), AttributeValue( i ) );
        	}
        }

        rule<ScannerT> const & start() const { return source; }

    private:

    	symbols<uint> typeSymbol;
    	symbols<SpeechPart> speechPart;
    	symbols<AttributeValue> attributeValue;

    	rule<ScannerT> patternName, wordType, source, wordBase, wordRestriction, matcherVariable, loopRestriction, patternRestrictions, bindingList, endLoop, endOptional, endBinding, endRestriction, alternativeTransformSource;

    	rule<ScannerT, AlternativeClosure::context_t> alternative;

    	rule<ScannerT, AgreementRestrictionClosure::context_t> agreementRestriction, matcherRestriction;
    	rule<ScannerT, DictionaryRestrictionClosure::context_t> dictionaryRestriction;

    	rule<ScannerT, MatcherClosure::context_t> matcher;
    	rule<ScannerT, WordMatcherClosure::context_t> wordMatcher;
    	rule<ScannerT, PatternMatcherClosure::context_t> patternMatcher;
    	rule<ScannerT, LoopMatcherClosure::context_t> loopMatcher;
    	rule<ScannerT, LoopBodyClosure::context_t> loopBody;
    	rule<ScannerT, TokenMatcherClosure::context_t> tokenMatcher;

    	rule<ScannerT, BindingClosure::context_t> binding;

    	rule<ScannerT, PatternClosure::context_t> pattern;

    	rule<ScannerT, ExpressionClosure::context_t> expression, localExpression;
    	rule<ScannerT> propertyExpression, stringLiteralExpression, literalExpression;

    	VariableParser variable;
    	AttributeKeyParser attributeKey;
    };

    ParserImpl( NamespaceRef space, const std::map<std::string, transforms::TransformBuilderRef>& tbs ) : Parser( space, tbs ) {}
    ~ParserImpl() {}

	PatternBuilder::BuildInfo buildNoException(const char * str ){
		try {
			parse_info<const char *> pi = boost::spirit::classic::parse( str, *this, space_p );
			PatternBuilder::BuildInfo bi;
			bi.parseLength = (uint) pi.length;
			bi.parseTail = pi.stop;
		    return bi;
		} catch ( parser_error<Errors,const char *> & err ) {
			PatternBuilder::BuildInfo errBi;
			errBi.parseLength = -1;
			errBi.parseTail = str;
			switch( err.descriptor ) {
			case BindingEndMissing:
				errBi.errorMsg =  "Binding end missing";
			case RestrictionEndMissing:
				errBi.errorMsg = "Restriction end missing";
			case LoopEndMissing:
				errBi.errorMsg = "Loop end missing";
			case OptionalEndMissing:
				errBi.errorMsg = "Optional group end missing";
			case NoMatchersInAlternative:
				errBi.errorMsg = "No matchers in alternative" ;
			case NoMatchersInGroup:
				errBi.errorMsg = "No matchers in group";
			case NoRestrictionBody:
				errBi.errorMsg = "No valid restrictions" ;
			case InvalidPatternName:
				errBi.errorMsg = "Invalid pattern name";
			case ClosingSglQuoteMissed:
				errBi.errorMsg = "Closing single quote missed";
			case ClosingDblQuoteMissed:
				errBi.errorMsg =  "Closing double quote missed";
			case AttributeValueExpected:
				errBi.errorMsg = "Invalid or no attribute value";
			default:
				errBi.errorMsg = (boost::format( "Error parsing template: %1%. Descriptor: %2%. Where: %3%" ) % err.what() % err.descriptor % err.where).str();
			}
			return errBi;
		} catch ( const std::exception & e ) {
			PatternBuilder::BuildInfo errBi;
			errBi.parseLength = -1;
			errBi.parseTail = str;
			errBi.errorMsg= (boost::format( "Error parsing template: %1% in template %2%" ) % e.what() % str).str();
			return errBi;
		} catch ( ... ) {
			PatternBuilder::BuildInfo errBi;
			errBi.parseLength = -1;
			errBi.parseTail = str;
			errBi.errorMsg = "Unknown error during parsing template";
			return errBi;
		}
	}

    PatternBuilder::BuildInfo build( const char * str ) throw (PatternBuildingException) {
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
    		throw PatternBuildingException( (boost::format( "Error parsing template: %1% in template %2%" ) % e.what() % str).str() );
    	} catch ( ... ) {
    		throw PatternBuildingException( "Unknown error during parsing template" );
    	}
    }
};

PatternBuilder::PatternBuilder( const NamespaceRef & ns, transforms::TransformBuilderRef defaultTransformBuilder ) :
    space( ns ),
    parser( new ParserImpl( space, transformBuilders ) ) {
    transformBuilders.insert(std::make_pair("", defaultTransformBuilder));
    transformBuilders.insert(std::make_pair("text", new transforms::TextTransformBuilder( space )));
    transformBuilders.insert(std::make_pair("pattern", new transforms::PatternTransformBuilder( space )));
}

PatternBuilder::PatternBuilder( const NamespaceRef & ns ) : PatternBuilder(ns, new transforms::DummyTransformBuilder()) {
    // Just delegate to two-arg constructor with dummy transform as default
}

PatternBuilder::~PatternBuilder() {
}

PatternBuilder::BuildInfo PatternBuilder::build( const std::string & str ) throw (PatternBuildingException) {
    return parser->build( str.c_str() );
}

PatternBuilder::BuildInfo PatternBuilder::buildNoException(const std::string &str){
    return parser->buildNoException(str.c_str());
}

} } // namespace lspl::patterns
