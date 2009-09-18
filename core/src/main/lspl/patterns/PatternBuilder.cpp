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

#include <boost/spirit/core.hpp>
#include <boost/spirit/attribute.hpp>
#include <boost/spirit/symbols.hpp>
#include <boost/spirit/error_handling/exceptions.hpp>
#include <boost/spirit/utility/functor_parser.hpp>
#include <boost/spirit/utility/confix.hpp>
#include <boost/spirit/utility/lists.hpp>
#include <boost/spirit/utility/distinct.hpp>
#include <boost/spirit/dynamic/switch.hpp>
#include <boost/spirit/phoenix/binders.hpp>
#include <boost/spirit/phoenix/functions.hpp>

using namespace boost::spirit;
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


	struct MatcherRestrictionClosure : public boost::spirit::closure< MatcherRestrictionClosure, AttributeKey, AttributeValue > {
		member1 attribute;
		member2 value;
	};

	struct PatternRestrictionClosure : public boost::spirit::closure< PatternRestrictionClosure, std::vector<std::pair<Variable,AttributeKey> >, Variable, AttributeKey > {
		member1 elements;
		member2 curVariable;
		member3 curAttribute;
	};

	struct DictionaryRestrictionClosure : public boost::spirit::closure< DictionaryRestrictionClosure, std::string, std::vector<Variable> > {
		member1 dictionaryName;
		member2 variables;
	};

	struct MatcherClosure : public boost::spirit::closure< MatcherClosure, uint, boost::ptr_vector<Restriction> > {
		member1 index;
		member2 restrictions;
	};

	struct WordMatcherClosure : public boost::spirit::closure< WordMatcherClosure, std::string, SpeechPart > {
		member1 base;
		member2 speechPart;
	};

	struct TokenMatcherClosure : public boost::spirit::closure< TokenMatcherClosure, std::string > {
		member1 token;
	};

	struct PatternMatcherClosure : public boost::spirit::closure< PatternMatcherClosure, std::string > {
		member1 name;
	};

	struct LoopMatcherClosure : public boost::spirit::closure< LoopMatcherClosure, uint, uint, std::vector<uint> > {
		member1 min;
		member2 max;
		member3 alternativesCount;
	};

	struct LoopBodyClosure : public boost::spirit::closure< LoopBodyClosure, uint > {
		member1 matcherCount;
	};

	struct PatternClosure : public boost::spirit::closure< PatternClosure, std::string, boost::ptr_vector<Alternative> > {
		member1 name;
		member2 alternatives;
	};

	struct AlternativeClosure : public boost::spirit::closure< AlternativeClosure, uint, boost::ptr_vector<Matcher>, boost::ptr_map<AttributeKey,Expression>, std::string > {
		member1 stub;
		member2 matchers;
		member3 bindings;
		member4 transformSource;
	};

	struct BindingClosure : public boost::spirit::closure< BindingClosure, AttributeKey, Expression * > {
		member1 att;
		member2 exp;
	};

	struct ExpressionClosure : public boost::spirit::closure< ExpressionClosure, Expression*, boost::ptr_vector<Expression> > {
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

        	function<AddMatcherRestrictionImpl> addMatcherRestriction;
        	function<AddPatternRestrictionImpl> addPatternRestriction;

        	function<AddPatternMatcherImpl> addPatternMatcher( AddPatternMatcherImpl( *self.space, typeSymbol ) );
        	function<AddWordMatcherImpl> addWordMatcher;
        	function<AddTokenMatcherImpl> addTokenMatcher;
        	function<AddLoopMatcherImpl> addLoopMatcher;
        	function<AddAlternativeDefinitionImpl> addAlternativeDefinition( AddAlternativeDefinitionImpl( *self.transformBuilder ) );
        	function<AddPatternDefinitionImpl> addPatternDefinition( AddPatternDefinitionImpl( *self.space, typeSymbol, *self.transformBuilder ) );
        	function<AddDictionaryRestrictionImpl> addDictionaryRestriction( *self.space );

        	function<CreateVariableExpression> createVariableExpression;
        	function<CreateAttributeExpression> createAttributeExpression;
        	function<CreateConcatExpression> createConcatExpression;

        	endLoop = expect_loop_end( ch_p( '}' ) );
        	endOptional = expect_optional_end( ch_p( ']' ) );
        	endBinding = expect_binding_end( ch_p(')') );
        	endRestriction = expect_restriction_end( ch_p('>') );

        	source = *pattern;

        	pattern = (
        			( patternName[ pattern.name = construct_<std::string>( arg1, arg2 ) ] >> '=' >> ( alternative % '|' ) ) |
        			( ( alternative % '|' )[ pattern.name = construct_<std::string>( arg1, arg2 ) ] >> !( ch_p('=') >> expect_valid_pattern_name( nothing_p ) ) )
        		)[ addPatternDefinition( pattern.name, pattern.alternatives ) ];

        	alternative = ( matcher >> *(matcher|restrictions) >> !bindingList >> !alternativeTransformSource )
        		[ addAlternativeDefinition( pattern.alternatives, alternative.matchers, alternative.bindings, construct_<std::string>( arg1, arg2 ), alternative.transformSource ) ];

        	alternativeTransformSource = str_p("=>") >> lexeme_d[ *~chset_p("\n|") ][ alternative.transformSource = construct_<std::string>( arg1, arg2 ) ];

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
        	    		( wordBase >> !(chset_p(";,") >> (matcherRestriction % ',')) ) |
        	    		( matcherRestriction % ',' )
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

        	patternMatcher = ( patternName[ patternMatcher.name = construct_<std::string>( arg1, arg2 ) ] >> matcherVariable >> !( '<' >> matcherRestriction % ',' >> endRestriction ) )
        		[ addPatternMatcher( alternative.matchers, patternMatcher.name, matcher.index, matcher.restrictions ) ];

        	/*
        	 * Парсер сопоставителя циклов
        	 */
        	loopMatcher = switch_p[
	        		case_p< '{' >( ( loopBody % '|' ) >> endLoop[ loopMatcher.min = 0 ][ loopMatcher.max = 0 ] >> !loopRestriction ),
	        		case_p< '[' >( ( loopBody % '|' ) >> endOptional[ loopMatcher.min = 0 ][ loopMatcher.max = 1 ] )
	        	][ addLoopMatcher( alternative.matchers, loopMatcher.min, loopMatcher.max, loopMatcher.alternativesCount ) ];

        	loopBody = expect_grp_matcher( matcher[ loopBody.matcherCount = 1 ] ) >> ( *(matcher[ loopBody.matcherCount ++ ] | restrictions) )[ add( loopMatcher.alternativesCount, loopBody.matcherCount ) ];

        	loopRestriction = '<' >> epsilon_p(chset_p("0-9,")) >> !uint_p[ loopMatcher.min = arg1 ] >> !( ',' >> uint_p[ loopMatcher.max = arg1 ] ) >> endRestriction;

        	/*
        	 * Парсеры ограничений
        	 */

        	restrictions = '<' >> expect_restriction_body( ( patternRestriction | dictionaryRestriction ) % ',' ) >> endRestriction;

        	matcherRestriction = ( attributeKey[ matcherRestriction.attribute = arg1 ] >> '=' >> expect_attribute_value( attributeValue[ matcherRestriction.value = arg1 ] ) )
        		[ addMatcherRestriction( matcher.restrictions, matcherRestriction.attribute, matcherRestriction.value ) ];

        	patternRestriction = (
        			( variable[ patternRestriction.curVariable = arg1 ] >> !( '.' >> attributeKey[ patternRestriction.curAttribute = arg1 ] ) )
        			[ add( patternRestriction.elements, construct_< std::pair<Variable,AttributeKey> >( patternRestriction.curVariable, patternRestriction.curAttribute ) ) ] % '='
        		)[ addPatternRestriction( alternative.matchers, patternRestriction.elements ) ];

        	dictionaryRestriction = ( ( lexeme_d[ +chset_p("a-zA-Z") ][ dictionaryRestriction.dictionaryName = construct_<std::string>( arg1, arg2 ) ] ) >> "(" >> ( variable[ add( dictionaryRestriction.variables, arg1 ) ] % "," ) >> ")" )[ addDictionaryRestriction( alternative.matchers, dictionaryRestriction.dictionaryName, dictionaryRestriction.variables ) ];

        	expression = ~eps_p( str_p( "AS" ) ) >> variable[ expression.exp = createVariableExpression( arg1 ) ] >>
				*( '.' >> attributeKey[ expression.exp = createAttributeExpression( expression.exp, arg1 ) ] ) >>
        		*( expression[ expression.exp = createConcatExpression( expression.exp, arg1 ) ] );

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
        }

        rule<ScannerT> const & start() const { return source; }

    private:

    	symbols<uint> typeSymbol;
    	symbols<SpeechPart> speechPart;
    	symbols<AttributeValue> attributeValue;

    	rule<ScannerT> patternName, wordType, source, wordBase, wordRestriction, matcherVariable, loopRestriction, restrictions, bindingList, endLoop, endOptional, endBinding, endRestriction, alternativeTransformSource;

    	rule<ScannerT, AlternativeClosure::context_t> alternative;

    	rule<ScannerT, MatcherRestrictionClosure::context_t> matcherRestriction;
    	rule<ScannerT, PatternRestrictionClosure::context_t> patternRestriction;
    	rule<ScannerT, DictionaryRestrictionClosure::context_t> dictionaryRestriction;

    	rule<ScannerT, MatcherClosure::context_t> matcher;
    	rule<ScannerT, WordMatcherClosure::context_t> wordMatcher;
    	rule<ScannerT, PatternMatcherClosure::context_t> patternMatcher;
    	rule<ScannerT, LoopMatcherClosure::context_t> loopMatcher;
    	rule<ScannerT, LoopBodyClosure::context_t> loopBody;
    	rule<ScannerT, TokenMatcherClosure::context_t> tokenMatcher;

    	rule<ScannerT, BindingClosure::context_t> binding;

    	rule<ScannerT, PatternClosure::context_t> pattern;

    	rule<ScannerT, ExpressionClosure::context_t> expression;

    	VariableParser variable;
    	AttributeKeyParser attributeKey;
    };

    ParserImpl( NamespaceRef space, transforms::TransformBuilderRef tb ) : Parser( space, tb ) {}
    ~ParserImpl() {}

    PatternBuilder::BuildInfo build( const char * str ) throw (PatternBuildingException) {
    	try {
    		parse_info<const char *> pi = boost::spirit::parse( str, *this, space_p );

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
        		throw PatternBuildingException( "Error parsing template" );
    		}
    	} catch ( ... ) {
    		throw PatternBuildingException( "Error parsing template" );
    	}
    }
};

PatternBuilder::PatternBuilder() : space( new Namespace() ), transformBuilder( new transforms::DummyTransformBuilder() ), parser( new ParserImpl( space, transformBuilder ) ) {
}

PatternBuilder::PatternBuilder( const NamespaceRef & ns ) : space( ns ), transformBuilder( new transforms::DummyTransformBuilder() ), parser( new ParserImpl( space, transformBuilder ) ) {
}

PatternBuilder::PatternBuilder( const NamespaceRef & ns, const transforms::TransformBuilderRef & tb ) : space( ns ), transformBuilder( tb ), parser( new ParserImpl( space, transformBuilder ) ) {
}


PatternBuilder::~PatternBuilder() {
}

PatternBuilder::BuildInfo PatternBuilder::build( const std::string & str ) throw (PatternBuildingException) {
	return parser->build( str.c_str() );
}

} } // namespace lspl::patterns
