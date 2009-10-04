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


using lspl::patterns::Pattern;
using lspl::patterns::matchers::Context;
using lspl::patterns::matchers::Matcher;
using lspl::patterns::matchers::WordMatcher;
using lspl::patterns::matchers::PatternMatcher;

using lspl::text::attributes::SpeechPart;

using lspl::text::indexes::Index;

using namespace lspl::text::markup;

static lspl::text::MatchList NO_MATCHES;
static lspl::text::markup::WordList NO_WORDS;

LSPL_REFCOUNT_CLASS( lspl::text::Text )

namespace lspl { namespace text {

template <class T>
inline void addTo( MatchList * matches, std::vector< base::Reference<T> > transitions ) {
	if ( !matches ) // Если передан пустой указатель, ничего не копируем
		return;

	matches->reserve( matches->size() + transitions.size() );

	for ( uint i = 0; i < transitions.size(); ++ i )
		matches->push_back( cast<Match>( transitions[i] ) );
}

Text::Text() {
}

Text::~Text() {
}

void Text::addWordToMarkup( const markup::WordRef & word ) {
	const_cast<Node&>( word->start ).transitions.push_back( word ); // Добавляем слово в разметку

	speechPartIndex.addAnnotation( SpeechPart::WORD, word.get() ); // Добавляем слово в индекс слов

	if ( word->getSpeechPart() != SpeechPart::WORD ) // Если слово имеет часть речи
		speechPartIndex.addAnnotation( word->getSpeechPart(), word.get() );
}

void Text::addMatchToMarkup( const MatchRef & match ) {
	const_cast<Node&>( match->start ).transitions.push_back( match ); // Добавляем сопоставлениев в разметку

	patternIndex.addAnnotation( &match->getPattern(), match.get() ); // Добавляем слово в индекс слов
}

void Text::addTokenToMarkup( const TokenRef & token ) {
	const_cast<Node&>( token->start ).transitions.push_back( token );
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

bool Text::prepareIndices( const Pattern & pattern, IndexIteratorsList & iterators ) {
	using namespace lspl::patterns;

	for ( uint i = 0; i < pattern.alternatives.size(); ++ i ) {
		const Alternative & alternative = pattern.alternatives[i];

		const boost::ptr_vector<IndexInfo> & indexInfos = alternative.getStartIndices();

		if ( indexInfos.empty() ) // Если для альтернативы не определеныин индексы, придется производить поиск без индексов
			return false;

		for ( uint j = 0; j < indexInfos.size(); ++j ) {
			const IndexInfo & info = indexInfos[j];

			switch ( info.type ) {
			case IndexInfo::WORD: {
				Index::Iterator * it = speechPartIndex.createIterator( static_cast<const WordIndexInfo &>( info ).speechPart );

				if ( !it ) {
					return false;
					std::cerr << "WARN: No speech part index found for " << static_cast<const WordIndexInfo &>( info ).speechPart.getAbbrevation() << std::endl;
				}

				iterators.push_back( std::make_pair( i, it ) );

				break;
			}
			case IndexInfo::PATTERN: {
				Index::Iterator * it = patternIndex.createIterator( &static_cast<const PatternIndexInfo &>( info ).pattern );

				if ( !it ) { // Шаблон еще не найден
					return false;
				}

				iterators.push_back( std::make_pair( i, it ) );

				break;
			}
			default:
				throw std::logic_error("Unknown index type");
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
		foreach ( const patterns::Alternative & alt, pattern.alternatives )
			matcher.buildTransitions( *nodes[nodeIndex], pattern, alt, context, results );

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
		Index::Iterator * index = 0;
		uint alternative;

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

		matcher.buildTransitions( *index->get(), pattern, pattern.alternatives[ alternative ], context, results ); // Производим поиск начиная с текущего индекса

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
