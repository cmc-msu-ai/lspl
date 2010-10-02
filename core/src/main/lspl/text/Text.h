#ifndef _LSPL_TEXT_TEXT_H_
#define _LSPL_TEXT_TEXT_H_

#include <ostream>
#include <set>
#include <vector>

#include "../base/Base.h"
#include "../base/RefCountObject.h"
#include "../base/Exception.h"

#include "Forward.h"
#include "Node.h"
#include "Match.h"
#include "markup/Word.h"
#include "TextConfig.h"

#include "../patterns/Forward.h"
#include "../patterns/restrictions/Restriction.h"

#include "attributes/SpeechPart.h"

#include "indexes/PatternIndex.h"
#include "indexes/SpeechPartIndex.h"

namespace lspl { namespace text {

class TextBuildingException : public base::Exception {
public:
	TextBuildingException( const std::string & description, const std::string & sourceFileName, int sourceLineNumber ) :
		Exception( description, sourceFileName, sourceLineNumber ) {}

	TextBuildingException( const std::string & description ) :
		Exception( description ) {}

	~TextBuildingException() throw() {}
};

/**
 * Объект, представляющий анализируемый текст в системе.
 *
 * Текст является немодифицируемым объектом, т.е. он один раз создается с помощью
 * объекта TextBuilder и после этого не меняет своего внутреннего содержимого.
 */
class LSPL_EXPORT Text : public base::RefCountObject, public base::IdentifiedObject<Text> {
public:
	Text();
	virtual ~Text();

	/**
	 * Получить содержимое текста
	 */
	const std::string & getContent() const { return content; }

	/**
	 * Получить конфигурацию текста
	 */
	const TextConfig & getConfig() const { return config; }

	/**
	 * Получить вектор всех узлов текста
	 */
	const std::vector< NodeRef > & getNodes() const {
		return nodes;
	}

	/**
	 * Получить вектор всех слов текста
	 */
	const markup::WordList & getWords() const {
		return *speechPartIndex.getWords( attributes::SpeechPart::WORD );
	}

	/**
	 * Получить вектор слов, принадлежащих заданной части речи
	 * @param sp часть речи
	 */
	const markup::WordList & getWords( attributes::SpeechPart sp ) const {
		return *speechPartIndex.getWords( sp );
	}

	/**
	 * Получить вектор сопоставлений для заданного шаблона
	 * @param pattern шаблон
	 */
	const MatchList & getMatches( const patterns::Pattern & pattern );

	/**
	 * Получить вектор сопоставлений для заданного шаблона
	 * @param pattern указатель на шаблон
	 */
	const MatchList & getMatches( const patterns::PatternRef & pattern ) {
		return getMatches( *pattern );
	}

	RestrictedMatchList getRestrictedMatches( const patterns::Pattern & pattern, const patterns::restrictions::Restriction & r );

	RestrictedMatchList getRestrictedMatches( const patterns::PatternRef & pattern, const patterns::restrictions::Restriction & r ) {
		return getRestrictedMatches( *pattern, r );
	}

	/**
	 * Проверить, готовы ли сопоставления шаблона к выдаче, или же необходимо провести процесс поиска
	 * @param pattern ссылка на шаблон
	 */
	bool isMatchesReady( const patterns::Pattern & pattern ) const;

	/**
	 * Проверить, готовы ли сопоставления шаблона к выдаче, или же необходимо провести процесс поиска
	 * @param pattern указатель на шаблон
	 */
	template <class PatternType>
	bool isMatchesReady( const base::Reference<PatternType> & pattern ) {
		return isMatchesReady( *pattern );
	}

	/**
	 * Сбросить в поток отладочное представление текста
	 *
	 * @param out поток в который сбрасывать дамп
	 * @param tabs участок, добавляемый в начало каждой строки (используется для структурного представления когда дамп вызывается из дампа вышестоящего объекта)
	 */
	void dump( std::ostream & out, std::string tabs = "" ) const;

private:
	friend class lspl::text::TextBuilder;

	typedef std::vector< std::pair< uint, indexes::Index::Iterator * > > IndexIteratorsList;

	/**
	 * Добавить лексему в разметку текста
	 */
	void addTokenToMarkup( const markup::TokenRef & token );

	/**
	 * Добавить слово в разметку текста
	 */
	void addWordToMarkup( const markup::WordRef & word );

	/**
	 * Добавить сопоставление в разметку текста
	 */
	void addMatchToMarkup( const MatchRef & match );

	/**
	 * Создать узел в тексте
	 * @param startOffset позиция начала узла в тексте
	 * @param endOffset позиция окончания узла в тексте
	 */
	NodeRef createNode( uint startOffset, uint endOffset );

	bool prepareIndices( const patterns::Pattern & pattern, IndexIteratorsList & iterators );
	void freeIndices( IndexIteratorsList & iterators );
	void processWithoutIndices( const patterns::Pattern & pattern );
	void processWithIndices( const patterns::Pattern & pattern, IndexIteratorsList & iterators );
public:

    /**
     * Количество объектов в памяти
     */
    static uint aliveObjectsCount;

private:

	/**
	 * Собственно текст
	 */
	std::string content;

	/**
	 * Конфигурация с которой был построен текст
	 */
	TextConfig config;

	/**
	 * Узлы текста
	 */
	std::vector< NodeRef > nodes;

	/**
	 * Множество идентификаторов обработанных шаблоноы
	 */
	std::set< uint > processedPatterns;

	indexes::PatternIndex patternIndex;
	indexes::SpeechPartIndex speechPartIndex;
};

} } // namespace lspl::text

#endif /*_LSPL_TEXT_TEXT_H_*/
