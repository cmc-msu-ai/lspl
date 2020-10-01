#include <complex>
#include "../../base/BaseInternal.h"
#include "JsonTextReader.h"

#include <complex>

#include "../../patterns/Pattern.h"
#include "../../patterns/matchers/Matcher.h"

#include "../TextConfig.h"
#include "../TextBuilder.h"

#include "../markup/Word.h"
#include "../Match.h"

#define PHOENIX_LIMIT 4
#define BOOST_SPIRIT_CLOSURE_LIMIT 4
#define BOOST_SPIRIT_SWITCH_CASE_LIMIT 5

#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_attribute.hpp>
#include <boost/spirit/include/classic_symbols.hpp>
#include <boost/spirit/include/classic_utility.hpp>
#include <boost/spirit/include/classic_dynamic.hpp>
#include <boost/spirit/include/classic_iterator.hpp>
#include <boost/spirit/include/phoenix1.hpp>

using namespace boost::spirit::classic;
using namespace phoenix;

using namespace lspl::text::attributes;
using namespace lspl::text::markup;

namespace lspl { namespace text { namespace readers {

class JsonTextReader::Parser : public grammar<JsonTextReader::Parser> {
public:

	struct ConfigClosure : public boost::spirit::classic::closure< ConfigClosure, int, int, int > {
		member1 analyzePunctuation;
		member2 analyzeSpaces;
		member3 splitToSentences;
	};

	struct NodeClosure : public boost::spirit::classic::closure< NodeClosure, uint, uint > {
		member1 startOffset;
		member2 endOffset;
	};

	struct AnnotationClosure : public boost::spirit::classic::closure< AnnotationClosure, uint, uint > {
		member1 start;
		member2 end;
	};

	struct WordClosure : public boost::spirit::classic::closure< WordClosure, std::string, std::string, uint, uint64 > {
		member1 token;
		member2 base;
		member3 speechPart;
		member4 attributes;
	};

	struct MatchClosure : public boost::spirit::classic::closure< MatchClosure, std::string, Match::AttributesMap> {
		member1 patternName;
		member2 attributes;
	};

	struct AttributesClosure : public boost::spirit::classic::closure< AttributesClosure, Match::AttributesMap > {
		member1 attributes;
	};

	struct AttributeClosure : public boost::spirit::classic::closure< AttributeClosure, std::string, attributes::AttributeValue > {
		member1 name;
		member2 value;
	};

	struct SetContentImpl {
		template <typename Arg1, typename Arg2>
		struct result { typedef void type; };

		SetContentImpl( const Parser & parser ) :
			parser( const_cast<Parser&>( parser ) ) {
		}

		void operator()( const char * start, const char * end ) const {
			parser.builder.setContent( std::string( start, end ) );
		}
	private:
		Parser & parser;
	};

	struct SetConfigImpl {
		template <typename Arg1, typename Arg2, typename Arg3>
		struct result { typedef void type; };

		SetConfigImpl( const Parser & parser ) :
			parser( const_cast<Parser&>( parser ) ) {
		}

		void operator()( int analyzePunctuation, int analyzeSpaces, int splitToSentences ) const {
			TextConfig config;
			config.analyzePunctuation = analyzePunctuation != 0;
			config.analyzeSpaces = analyzeSpaces != 0;
			config.splitToSentences = splitToSentences != 0;
			parser.builder.setConfig( config );
		}
	private:
		Parser & parser;
	};

	struct AddNodeImpl {
		template <typename Arg1, typename Arg2>
		struct result { typedef void type; };

		AddNodeImpl( const Parser & parser ) :
			parser( const_cast<Parser&>( parser ) ) {
		}

		void operator()( uint startOffset, uint endOffset ) const {
			parser.builder.createNode( startOffset, endOffset );
		}
	private:
		Parser & parser;
	};

	struct AddWordImpl {
		template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
		struct result { typedef void type; };

		AddWordImpl( const Parser & parser ) :
			parser( const_cast<Parser&>( parser ) ) {
		}

		void operator()( uint start, uint end, const std::string & token, const std::string & base, uint speechPart, uint64 attributes ) const {
			parser.builder.addWord( new Word( parser.builder.getNodeByIndex( start ), parser.builder.getNodeByIndex( end ), new Token(parser.builder.getNodeByIndex( start ), parser.builder.getNodeByIndex( end ), token ), base, "", attributes::SpeechPart( speechPart ), attributes ) );
		}
	private:
		Parser & parser;
	};

	struct AddMatchImpl {
		template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
		struct result { typedef void type; };

		AddMatchImpl( const Parser & parser ) :
			parser( const_cast<Parser&>( parser ) ) {
		}

		void operator()( uint start, uint end, const std::string & patternName, const Match::AttributesMap & attributes ) const {
			patterns::Pattern & p = *parser.ns.getPatternByName( patternName );
			parser.builder.addMatch( new Match( parser.builder.getNodeByIndex( start ), parser.builder.getNodeByIndex( end ), p, new MatchVariant( *p.getAlternatives()[0] ), attributes ) ); // TODO Transitions
		}
	private:
		Parser & parser;
	};

	struct GetIndexedAttributeValueImpl {
		template <typename Arg1>
		struct result { typedef AttributeValue type; };

		AttributeValue operator()( uint index ) const {
			return AttributeValue( index );
		}
	};

	struct GetStringAttributeValueImpl {
		template <typename Arg1,typename Arg2>
		struct result { typedef AttributeValue type; };

		AttributeValue operator()( const char * begin, const char * end ) const {
			return AttributeValue( std::string( begin, end ) );
		}
	};

	struct AddAttributeToSetImpl {
		template <typename Arg1,typename Arg2, typename Arg3>
		struct result { typedef void type; };

		void operator()( Match::AttributesMap & atts, const std::string & attKeyAbbr, const AttributeValue & attVal ) const {
			AttributeKey attKey = AttributeKey::findByAbbrevation( attKeyAbbr );

			if ( attKey == AttributeKey::UNDEFINED )
				attKey = AttributeKey::create( attKeyAbbr, attKeyAbbr, attKeyAbbr );

			atts.insert( std::make_pair( attKey, attVal ) );
		}
	};

	template <typename ScannerT> class definition {
	public:
		definition( const Parser & self ) {
			document = "{" >> ( ( config | content | nodes | annotations ) % "," ) >> "}";

			defineConfig( self );
			defineNodes( self );
			defineAnnotations( self );
			defineContent( self );
			defineAttributes( self );
		}

		void defineContent( const Parser & self ) {
			function<SetContentImpl> setContent( self );

			content = str_p( "content" ) >> ":" >> "'" >> ( *~ch_p('\'') )[ setContent( arg1, arg2 ) ] >> "'";
		}

		void defineConfig( const Parser & self ) {
			function<SetConfigImpl> setConfig( self );

			config = ( str_p("config") >> ":" >> "{" >> ( ( configAnalyzePunctuation | configAnalyzeSpaces | configSplitToSentences ) % "," ) >> "}" )[ setConfig( config.analyzePunctuation, config.analyzeSpaces, config.splitToSentences ) ];

			configAnalyzePunctuation = str_p( "analyzePunctuation" ) >> ":" >> int_p[ config.analyzePunctuation = arg1 ];
			configAnalyzeSpaces = str_p( "analyzeSpaces" ) >> ":" >> int_p[ config.analyzeSpaces = arg1 ];
			configSplitToSentences = str_p( "splitToSentences" ) >> ":" >> int_p[ config.splitToSentences = arg1 ];
		}

		void defineNodes( const Parser & self ) {
			function<AddNodeImpl> addNode( self );

			nodes = str_p("nodes") >> ":" >> "[" >> ( node % "," ) >> "]";
			node =  ("{" >> ( ( nodeStartOffset | nodeEndOffset ) % "," ) >> "}" )[ addNode( node.startOffset, node.endOffset ) ];

			nodeStartOffset = str_p( "startOffset" ) >> ":" >> int_p[ node.startOffset = arg1 ];
			nodeEndOffset = str_p( "endOffset" ) >> ":" >> int_p[ node.endOffset = arg1 ];
		}

		void defineAnnotations( const Parser & self ) {
			function<AddWordImpl> addWord( self );
			function<AddMatchImpl> addMatch( self );

			annotations = str_p("annotations") >> ":" >> "[" >> ( annotation % "," ) >> "]";

			annotation = "{" >> str_p( "type" ) >> ":" >> switch_p[
					case_p<'0'>( "," >> eps_p ), // Пробел
					case_p<'1'>( "," >> punct ), // Пунктуация
					case_p<'2'>( "," >> word ), // Слово
					case_p<'3'>( "," >> match ), // Сопоставление
					case_p<'4'>( "," >> eps_p ) // Цикл
				] >> "}";

			word = ( ( annotationStart | annotationEnd | wordToken | wordBase | wordSpeechPart | wordAttributes ) % "," )[ addWord( annotation.start, annotation.end, word.token, word.base, word.speechPart, word.attributes ) ];
			match = ( annotationStart | annotationEnd | matchPattern | matchAttributes ) % ",";
			punct = ( annotationStart | annotationEnd ) % ",";

			annotationStart = str_p( "start" ) >> ":" >> int_p[ annotation.start = arg1 ];
			annotationEnd = str_p( "end" ) >> ":" >> int_p[ annotation.end = arg1 ];

			wordToken = str_p( "token" ) >> ":" >> "'" >> (*~ch_p('\''))[ word.token = construct_<std::string>( arg1, arg2 ) ] >> "'";
			wordBase = str_p( "base" ) >> ":" >> "'" >> (*~ch_p('\''))[ word.base = construct_<std::string>( arg1, arg2 ) ] >> "'";
			wordSpeechPart = str_p( "speechPart" ) >> ":" >> int_p[ word.speechPart = arg1 ];
			wordAttributes = str_p( "attributes" ) >> ":" >> uint_parser<uint64, 10, 1, -1>()[ word.attributes = arg1 ];

			matchPattern = str_p( "pattern" ) >> ":" >> "'" >> (*~ch_p('\''))[ match.patternName = construct_<std::string>( arg1, arg2 ) ] >> "'";
			matchAttributes = str_p( "attributes" ) >> ":" >> attributes[ match.attributes = arg1 ];
		}

		void defineAttributes( const Parser & self ) {
			function<GetIndexedAttributeValueImpl> getIndexedAttributeValue;
			function<GetStringAttributeValueImpl> getStringAttributeValue;
			function<AddAttributeToSetImpl> addAttributeToSet;

			attributes = "{" >> ( attribute % "," ) >> "}";
			attribute = ( lexeme_d[ +chset_p("a-zA-Z") ][ attribute.name = construct_<std::string>( arg1, arg2 ) ] >> ":" >> ( indexedAttributeValue | stringAttributeValue | compoundAttributeValue ) )[ addAttributeToSet( attributes.attributes, attribute.name, attribute.value ) ];

			indexedAttributeValue = uint_p[ attribute.value = getIndexedAttributeValue( arg1 ) ];
			stringAttributeValue = "'" >> (*~ch_p('\''))[ attribute.value = getStringAttributeValue( arg1, arg2 ) ] >> "'";
			compoundAttributeValue = nothing_p;
		}

		rule<ScannerT> const & start() const { return document; }

	private:
		rule<ScannerT> document, content;

		rule<ScannerT, ConfigClosure::context_t> config;
		rule<ScannerT> configAnalyzePunctuation, configAnalyzeSpaces, configSplitToSentences; // Правила разбора конфигурации

		rule<ScannerT, NodeClosure::context_t> node;
		rule<ScannerT> nodes, nodeStartOffset, nodeEndOffset;

		rule<ScannerT, AnnotationClosure::context_t> annotation;
		rule<ScannerT> annotations, punct;

		rule<ScannerT, WordClosure::context_t> word;
		rule<ScannerT> wordToken, wordBase, wordSpeechPart, wordAttributes;

		rule<ScannerT, MatchClosure::context_t> match;
		rule<ScannerT> matchPattern, matchAttributes;

		rule<ScannerT, AttributesClosure::context_t> attributes;
		rule<ScannerT, AttributeClosure::context_t> attribute;
		rule<ScannerT> indexedAttributeValue, stringAttributeValue, compoundAttributeValue;

		rule<ScannerT> annotationStart, annotationEnd;
	};

public:
	Parser( TextBuilder & builder, Namespace & ns ) :
		builder( builder ), ns( ns ) {
	}

	void parseFrom( std::istream & is ) {
		parseFrom( std::string( std::istreambuf_iterator<char>( is ), {} ).c_str() );
	}

	void parseFrom( const char * str ) {
		parse_info<const char *> pi = boost::spirit::classic::parse( str, *this, space_p );

		/*std::cout << "Parsed: " << pi.length << std::endl;
		std::cout << "Tail: " << pi.stop << std::endl;*/
	}

public:
	TextBuilder & builder;
	Namespace & ns;
};

JsonTextReader::JsonTextReader( const NamespaceRef & ns ) : ns( ns ) {
}

JsonTextReader::~JsonTextReader() {
}

TextRef JsonTextReader::readFromStream( std::istream & is ) {
	TextBuilder tb;
	Parser p( tb, *ns );

	tb.start();
	p.parseFrom( is );

	return tb.getText();
}

TextRef JsonTextReader::readFromStream( std::istream & is, const std::string & enc ) {
	return readFromStream( is );
}

} } } // namespace lspl::text::readers
