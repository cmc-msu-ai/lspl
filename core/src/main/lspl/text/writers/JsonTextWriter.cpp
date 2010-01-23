#include "../../base/BaseInternal.h"

#include "JsonTextWriter.h"
#include "../Text.h"
#include "../../patterns/Pattern.h"

using lspl::text::attributes::AttributeValue;
using lspl::text::attributes::AttributeType;
using namespace lspl::text::markup;

namespace lspl { namespace text { namespace writers {

JsonTextWriter::JsonTextWriter()
{
}

JsonTextWriter::~JsonTextWriter()
{
}

void JsonTextWriter::writeToStream( const Text & text, std::ostream & os ) {
	os << "{\n\tconfig: ";
	writeConfig( text.getConfig(), os );
	os << ",\n\tcontent: '" << text.getContent() << "',\n\tnodes: [\n\t\t";

	uint nodesCount = text.getNodes().size();
	if ( nodesCount > 0 ) {
		writeNode( *text.getNodes().at( 0 ), os );
		for ( uint i = 1; i < nodesCount; ++ i ) {
			os << ",\n\t\t";
			writeNode( *text.getNodes().at( i ), os );
		}
	}

	os << "\n\t],\n\tannotations: [\n\t\t";

	for ( uint i = 0; i < nodesCount; ++ i ) {
		const Node & node = *text.getNodes().at( i );

		for ( uint j = 0; j < node.getTransitionCount(); ++ j ) {
			if ( i != 0 || j != 0 )
				os << ",\n\t\t";
			writeAnnotation( *node.getTransition( j ), os );
		}
	}

	os << "\n\t]\n}";
}

void JsonTextWriter::writeConfig( const TextConfig & config, std::ostream & os ) {
	os  << "{ analyzePunctuation: " << config.analyzePunctuation
		<< ", analyzeSpaces: " << config.analyzeSpaces
		<< ", splitToSentences: " << config.splitToSentences << " }";
}

void JsonTextWriter::writeNode( const Node & node, std::ostream & os ) {
	os  << "{ startOffset: " << node.startOffset
		<< ", endOffset: " << node.endOffset << " }";
}

void JsonTextWriter::writeAnnotation( const Transition & ann, std::ostream & os ) {
	os	<< "{ id: " << ann.id
		<< ", type: " << ann.type
		<< ", start: " << ann.start.index
		<< ", end: " << ann.end.index;

	switch ( ann.type ) {
	case Transition::WORD:
		{
			const Word & word = dynamic_cast<const Word&>( ann );
			os	<< ", token: '" << word.getToken()
				<< "', base: '" << word.getBase()
				<< "', speechPart: " << word.getSpeechPart().id
				<< ", attributes: " << word.getAttributes();
			break;
		}
	case Transition::TOKEN:
		{
			const Token & token = dynamic_cast<const Token&>( ann );
			os	<< ", token: '" << token.getToken();
			break;
		}
	case Transition::SPACE:
		break;
	case Transition::MATCH:
		{
			const Match & match = dynamic_cast<const Match&>( ann );
			os	<< ", pattern: '" << match.getPattern().name
				<< "', attributes: { ";

			/*
			 TODO: Сериализация
			Match::AttributesMap::const_iterator it = match.attributes.begin();
			if ( it != match.attributes.end() ) {
				os	<< it->first.getAbbrevation() << ": ";

				writeAttributeValue( it->second, os );

				++ it;

				while ( it != match.attributes.end() ) {
					os	<< ", " << it->first.getAbbrevation() << ": ";

					writeAttributeValue( it->second, os );

					++ it;
				}
			}*/

			os << " }";

			break;
		}
	case Transition::LOOP:
		break;
	}

	os << " }";
}

void JsonTextWriter::writeAttributeValue( const AttributeValue & value, std::ostream & os ) {
	switch ( value.type.id ) {
	case AttributeType::INDEXED_ID:
		os << value.value;
		break;
	case AttributeType::STRING_ID:
		os << "'" << value.getString() << "'";
		break;
	case AttributeType::COMPOUND_ID:
		os << "{ ";

		// TODO Implement

		os << " }";
		break;
	}
}

} } }
