#include "../../base/BaseInternal.h"

#include "PlainTextReader.h"

#include "../TextBuilder.h"
#include "../../morphology/Morphology.h"
#include "../../graphan/Graphan.h"
#include "../../patterns/matchers/Matcher.h"
#include "../../utils/Conversion.h"

#include <string>

using namespace lspl::morphology;

using namespace lspl::text::attributes;
using namespace lspl::text::markup;

using lspl::utils::Conversion;

namespace lspl { namespace text { namespace readers {

PlainTextReader::PlainTextReader() {
}

PlainTextReader::~PlainTextReader() {
}

TextRef PlainTextReader::readFromStream( std::istream & is, const std::string & enc ) {
	const std::string buffer( std::istreambuf_iterator<char>( is ), {} );
	return readFromString( Conversion( enc, Conversion::DEFAULT_ENCODING ).convert( buffer ) );
}

TextRef PlainTextReader::readFromStream( std::istream & is ) {
	const std::string buffer( std::istreambuf_iterator<char>( is ), {} );
	return readFromString( buffer ); // Создаем новый экзмпляр текста
}

TextRef PlainTextReader::readFromString( const std::string & content, const std::string & enc ) {
	return readFromString( Conversion( enc, Conversion::DEFAULT_ENCODING ).convert( content ) );
}

TextRef PlainTextReader::readFromString( const std::string & content ) {
	boost::ptr_vector<graphan::Unit> units; // Лексемы
	boost::ptr_vector<morphology::WordForm> wordForms; // Массив для форм слова

	graphan::Graphan::instance().analyzeString( content, units );

	TextBuilder builder;
	builder.start(); // Создаем новый текст
	builder.setContent( content ); // Устанавливаем содержимое текста
	builder.setConfig( config ); // Устанавливаем конфигурацию текста

	Node * lastNode; // Создаем начальный узел текста
	const graphan::Unit * lastUnit = 0;
	uint lastUnitEnd = 0;

	for (size_t unitIndex = 1; unitIndex < units.size(); ++unitIndex) { // Пробегаем все строки графематической таблицы
		const graphan::Unit & unit = units[ unitIndex ]; // Извлекаем текущую лексема

		if ( unit.getType() == graphan::Unit::WORD || ( unit.getType() == graphan::Unit::PUNCT && config.analyzePunctuation ) ) {
			Node * nextNode = &builder.createNode( lastUnitEnd, unit.getOffset() ); // Создаем новый текстовый узел

			if (lastUnit)
				addTransitions( builder, *lastNode, *nextNode, *lastUnit, wordForms );

			lastUnit = &unit;
			lastUnitEnd = unit.getOffset() + unit.getLength();
			lastNode = nextNode; // Меняем текущий узел
		}
	}

	if (lastUnit) {
		addTransitions( builder, *lastNode, builder.createNode( lastUnitEnd, lastUnitEnd ), *lastUnit, wordForms );
	}

	return builder.getText(); // Возвращаем результат
}

void PlainTextReader::addTransitions( TextBuilder & builder, Node & start, Node & end, const graphan::Unit & unit, boost::ptr_vector<morphology::WordForm> & wordForms ) {
	Morphology & morphology = Morphology::instance();

	if ( unit.getType() == graphan::Unit::WORD ) {
		TokenRef token = new Token( start, end, unit.getString() ); // Получаем лексему

		builder.addToken( token );

		wordForms.clear(); // Очищаем список форм слова

		Morphology::instance().appendWordForms( token->getToken(), wordForms ); // Проводим морфологический анализ

		for ( uint formIndex = 0; formIndex < wordForms.size(); ++ formIndex ) {
			WordForm & form = wordForms[ formIndex ];

			for ( uint i = 0, e = form.getAttributeSetCount(); i < e; ++ i )
				builder.addWord( new Word( start, end, token, form.getBase(), form.getStem(), form.getSpeechPart(), form.getAttributeSet( i ) ) ); // Создаем новый переход-слово и добавляем в список переходов из узла
		}
	} else if ( unit.getType() == graphan::Unit::PUNCT && config.analyzePunctuation ) {
		builder.addToken( new Token( start, end, unit.getString() ) );
	}
}

} } } // namespace lspl::text::readers
