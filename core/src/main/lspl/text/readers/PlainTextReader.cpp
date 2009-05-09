#include "../../base/BaseInternal.h"

#include "PlainTextReader.h"

#include "../TextBuilder.h"
#include "../../morphology/Morphology.h"

#include "aot/Source/GraphanLib/GraphmatFile.h"

#include <string>

using namespace lspl::morphology;

using namespace lspl::text::attributes;
using namespace lspl::text::markup;

namespace lspl { namespace text { namespace readers {

PlainTextReader::PlainTextReader() {
}

PlainTextReader::~PlainTextReader() {
}

TextRef PlainTextReader::readFromStream( std::istream & is ) {
	is.seekg( 0, std::ios_base::end );
	int size = is.tellg();
	is.seekg( 0, std::ios_base::beg );

	// TODO Оптимизировать, не выделять
	char * buffer = new char[ size ];

	is.read( buffer, size );

	std::string content( buffer, size );

	delete[] buffer;

	return readFromString( content ); // Создаем новый экзмпляр текста
}

TextRef PlainTextReader::readFromString( const std::string & content ) {
	CGraphmatFile file;
	TextBuilder builder;

	Morphology::instance(); // Инициализируем морфологию

#ifdef MSVC
	if ( !file.LoadDicts() ) // Загружаем словари
		throw new TextBuildingException( file.GetLastError().c_str(), __FILE__, 0 );

	if ( !file.LoadStringToGraphan( content ) ) // Загружаем файл
		throw new TextBuildingException( file.GetLastError().c_str(), __FILE__, 0 );
#else
	if ( !file.LoadDicts() ) // Загружаем словари
		throw new TextBuildingException( file.GetLastError().c_str(), __FILE__, __LINE__ );

	if ( !file.LoadStringToGraphan( content ) ) // Загружаем файл
		throw new TextBuildingException( file.GetLastError().c_str(), __FILE__, __LINE__ );
#endif

	builder.start(); // Создаем новый текст
	builder.setContent( content ); // Устанавливаем содержимое текста
	builder.setConfig( config ); // Устанавливаем конфигурацию текста

	const std::vector<CGraLine> & units = file.GetUnits(); // Получаем графематическую таблицу

	Node * lastNode; // Создаем начальный узел текста
	const CGraLine * lastUnit = 0;
	uint lastUnitEnd = 0;

	WordFormList wordForms; // Массив для форм слова

	for (size_t unitIndex = 1; unitIndex < units.size(); ++unitIndex) { // Пробегаем все строки графематической таблицы
		const CGraLine & unit = units[ unitIndex ]; // Извлекаем текущую лексема

		if ( unit.IsWordOrNumberOrAbbr() || ( unit.IsPunct() && config.analyzePunctuation ) ) {
			Node * nextNode = &builder.createNode( lastUnitEnd, unit.GetInputOffset() ); // Создаем новый текстовый узел

			if (lastUnit)
				addTransitions( builder, *lastNode, *nextNode, *lastUnit, wordForms );

			lastUnit = &unit;
			lastUnitEnd = unit.GetInputOffset() + unit.GetTokenLength();
			lastNode = nextNode; // Меняем текущий узел
		}
	}

	if (lastUnit) {
		addTransitions( builder, *lastNode, builder.createNode( lastUnitEnd, lastUnitEnd ), *lastUnit, wordForms );
	}

	return builder.getText(); // Возвращаем результат
}

void PlainTextReader::addTransitions( TextBuilder & builder, Node & start, Node & end, const CGraLine & unit, WordFormList & wordForms ) {
	Morphology & morphology = Morphology::instance();

	const char * tokenStart = unit.GetToken(); // Получаем указатель на начало лексемы
	uint tokenLength = unit.GetTokenLength(); // Получаем длину лексемы

	if ( unit.IsWordOrNumberOrAbbr() ) {
		TokenRef token = new Token( start, end, std::string( tokenStart, tokenLength ) ); // Получаем лексему

		builder.addToken( token );

		wordForms.clear(); // Очищаем список форм слова

		Morphology::instance().appendWordForms( token->getToken(), wordForms ); // Проводим морфологический анализ

		for ( uint formIndex = 0; formIndex < wordForms.size(); ++ formIndex ) {
			WordFormRef form = wordForms[ formIndex ];

			for ( uint i = 0, e = form->getAttributeSetCount(); i < e; ++ i )
				builder.addWord( new Word( start, end, token, form->getBase(), form->getSpeechPart(), form->getAttributeSet( i ) ) ); // Создаем новый переход-слово и добавляем в список переходов из узла
		}
	} else if ( unit.IsPunct() && config.analyzePunctuation ) {
		builder.addToken( new Token( start, end, std::string( tokenStart, tokenLength ) ) );
	}
}

} } } // namespace lspl::text::readers
