#include "../base/BaseInternal.h"

#include <string>
#include <vector>
#include <iostream>

#include "Text.h"
#include "Node.h"
#include "Transition.h"
#include "markup/Word.h"
#include "Match.h"

#include "../patterns/Pattern.h"
#include "../patterns/matchers/Matcher.h"
#include "../patterns/matchers/WordMatcher.h"
#include "../patterns/matchers/PatternMatcher.h"

#include "../patterns/restrictions/Restriction.h"
#include "../transforms/ContextRetriever.h"

using lspl::patterns::Alternative;
using lspl::patterns::Pattern;
using lspl::patterns::matchers::Context;
using lspl::patterns::matchers::Matcher;
using lspl::patterns::matchers::Variable;
using lspl::patterns::matchers::WordMatcher;
using lspl::patterns::matchers::PatternMatcher;
using lspl::patterns::restrictions::Restriction;

using lspl::text::attributes::SpeechPart;

using lspl::text::indexes::Index;

using namespace lspl::text::markup;

static lspl::text::MatchList NO_MATCHES;
static lspl::text::markup::WordList NO_WORDS;

LSPL_REFCOUNT_CLASS( lspl::text::Text )

namespace lspl { namespace text {

uint Text::aliveObjectsCount = 0;

template <class T>
inline void addTo( MatchList * matches, std::vector< base::Reference<T> > transitions ) {
	if ( !matches ) // Если передан пустой указатель, ничего не копируем
		return;

	matches->reserve( matches->size() + transitions.size() );

	for ( uint i = 0; i < transitions.size(); ++ i )
		matches->push_back( cast<Match>( transitions[i] ) );
}

Text::Text() {
    ++ aliveObjectsCount;
}

Text::~Text() {
    -- aliveObjectsCount;
}

void Text::addWordToMarkup( const markup::WordRef & word ) {
	const_cast<Node&>( word->start ).addWordTransition( word ); // Добавляем слово в разметку

	speechPartIndex.addAnnotation( SpeechPart::WORD, word.get() ); // Добавляем слово в индекс слов

	if ( word->getSpeechPart() != SpeechPart::WORD ) // Если слово имеет часть речи
		speechPartIndex.addAnnotation( word->getSpeechPart(), word.get() );
}

void Text::addMatchToMarkup( const MatchRef & match ) {
	const_cast<Node&>( match->start ).addMatchTransition( match ); // Добавляем сопоставлениев в разметку

	patternIndex.addAnnotation( &match->getPattern(), match.get() ); // Добавляем слово в индекс слов
}

void Text::addTokenToMarkup( const TokenRef & token ) {
	const_cast<Node&>( token->start ).addTokenTransition( token );
}

const MatchList & Text::getMatches( const Pattern & pattern ) {
	if ( isMatchesReady( pattern ) ) // Если шаблон уже обработан в тексте, то выходим
		return *patternIndex.getMatches( &pattern ); // Возвращаем список сопоставлений из индекса

	patternIndex.addPattern( &pattern ); // Регистрируем шаблон в индексе

	IndexIteratorsList iterators; // Список итераторов инлексов

	if ( prepareIndices( pattern, iterators ) ) { // Если список индексов успешно построен
		processWithIndices( pattern, iterators ); // Обрабатываем шаблон с индексированием
	} else {
		processWithoutIndices( pattern ); // Обрабатываем шаблон без индексирования
	}

	freeIndices( iterators ); // Освобождаем память, занимаемую временными индексами

	processedPatterns.insert( pattern.id ); // Добавляем шаблон в список обработанных

	return *patternIndex.getMatches( &pattern ); // Возвращаем результаты из индекса
}

RestrictedMatchList Text::getRestrictedMatches( const Pattern & pattern, const Restriction & r ) {
	transforms::ContextRetriever contextRetriever; // Преобразование для построения контекста по варианту наложения
	RestrictedMatchList results;
	MatchList matches = getMatches( pattern );

	for( const MatchRef & match : matches ) {
		RestrictedMatchRef rm;

		for ( uint i = 0; i < match->getVariantCount(); ++ i ) {
			MatchVariantRef v = match->getVariant( i );
			Context ctx = contextRetriever.apply( *v ); // Вычисляем контекст для варианта

			if ( !r.matches( 0, Variable(), ctx ) ) // Если вариант не проходит ограничение
				continue; // То пропускаем его

			if ( rm )
				rm->addVariant( v.get() );
			else
				rm = new RestrictedMatch( match->start, match->end, pattern, v.get(), match->getAttributes() );
		}

		if ( rm ) results.push_back( rm ); // Если получен вариант, то добавляем его в список результатов
	}

	return results;
}

bool Text::prepareIndices( const Pattern & pattern, IndexIteratorsList & iterators ) {
	using namespace lspl::patterns;

	for ( uint i = 0; i < pattern.getAlternatives().size(); ++ i ) { // Перебираем все альтернативы шаблона
		const Alternative & alternative = *pattern.getAlternatives()[i]; // Получаем ссылку на альтернативу
		const std::vector<const Matcher*> & startMatchers = alternative.getStartMatchers(); // Получаем информацию о подходящих индексах из альтернативы


		if ( startMatchers.empty() ) // Если для альтернативы не определеныин начальные элементы, то это какая-та ошибка
			throw std::logic_error( "No start matchers" );

		for( const Matcher* matcher : startMatchers ) {
			switch ( matcher->type ) {
			case Matcher::WORD: {
				Index::Iterator * it = speechPartIndex.createIterator( static_cast<const WordMatcher *>( matcher )->speechPart );

				if ( !it )
					throw std::logic_error( "No speech part index" );

				iterators.push_back( std::make_pair( i, it ) );
				break;
			}
			case Matcher::PATTERN: {
				const Pattern & dep = static_cast<const PatternMatcher *>( matcher )->pattern;

				Index::Iterator * it = patternIndex.createIterator( &dep );

				if ( !it && !dep.dependsOn( pattern, true ) ) { // Шаблон еще не найден, и нет обратной зависимости
					getMatches( dep ); // Ищем зависимость
					it = patternIndex.createIterator( &dep ); // Переустанавливаем итератор
				}

				if ( !it ) // Шаблон все еще не найден
					return false;

				iterators.push_back( std::make_pair( i, it ) );
				break;
			}
			default:
				return false;
			}
		}
	}

	return true;
}

void Text::freeIndices( IndexIteratorsList & iterators ) {
	for ( uint i = 0; i < iterators.size(); ++ i )
		if ( iterators[i].second )
			delete iterators[i].second;
}

void Text::processWithoutIndices( const Pattern & pattern ) {
	TransitionList results;
	PatternMatcher matcher( pattern ); // Сопоставитель шаблона
	Context context; // Контекст сопоставления

	for ( uint nodeIndex = 0; nodeIndex < nodes.size(); ++ nodeIndex ) {
		for( const std::unique_ptr<Alternative> &alt : pattern.getAlternatives() ) // Перебираем все альтернативы шаблона
			matcher.buildTransitions( *nodes[nodeIndex], pattern, *alt, context, results );

		for ( uint i = 0; i < results.size(); ++ i )
			addMatchToMarkup( results[i].cast<Match>() );

		results.clear();
	}
}

void Text::processWithIndices( const Pattern & pattern, IndexIteratorsList & iterators ) {
	TransitionList results;
	PatternMatcher matcher( pattern ); // Сопоставитель шаблона
	Context context; // Контекст сопоставления

	const Node * lastNode = 0;

	while ( true ) {
		Index::Iterator * index = 0; // Используемый индекс
		uint alternative; // Индекс альтернативы

		for ( uint i = 0; i < iterators.size(); ++ i ) { // Находим ближайший индекс
			std::pair<uint,Index::Iterator *> & currentPair = iterators[ i ];

			if ( !currentPair.second->finished() && ( !index || currentPair.second->get()->start < index->get()->start ) ) {
				alternative = currentPair.first;
				index = currentPair.second;
			}
		}

		if ( !index || ( lastNode && &index->get()->start != lastNode ) ) { // Производим сброс сопоставлений
			for ( uint i = 0; i < results.size(); ++ i )
				addMatchToMarkup( results[i].cast<Match>() );

			results.clear();
		}

		if ( !index ) // Если не нашли подходящего индекса, значит поиск завершен
			return;

		matcher.buildTransitions( *index->get(), pattern, *pattern.getAlternatives()[ alternative ], context, results ); // Производим поиск начиная с текущего индекса

		lastNode = &index->get()->start;
		index->increment();
	}
}

bool Text::isMatchesReady( const Pattern & pattern ) const {
	 return processedPatterns.find( pattern.id ) != processedPatterns.end();
}

NodeRef Text::createNode( uint startOffset, uint endOffset ) {
	NodeRef node( new Node( (uint)nodes.size(), startOffset, endOffset, *this ) );

	nodes.push_back( node );

	return node;
}

void Text::dump(std::ostream & out, std::string tabs) const {
	out << "Text{ nodes = [\n\t" << tabs;

	for ( uint i = 0; i < nodes.size(); ++i) {
		if (i != 0)
			out << ",\n\t" << tabs;

		nodes[i]->dump(out, tabs + "\t");
	}

	out <<"\n" << tabs << "] }";
}

} } // namespace lspl::text
