#include "../../base/BaseInternal.h"

#include "PatternMatcher.h"

#include "../Pattern.h"

#include "../../text/attributes/SpeechPart.h"
#include "../../text/Match.h"
#include "../../text/Text.h"
#include "../../text/Node.h"

#include <stdexcept>
#include <memory>

using lspl::text::attributes::SpeechPart;

using lspl::text::Node;
using lspl::text::Transition;
using lspl::text::TransitionRef;
using lspl::text::TransitionList;
using lspl::text::Match;
using lspl::text::MatchVariant;

namespace lspl { namespace patterns { namespace matchers {

struct PatternMatchState {

	const Node & startNode;

	Context context;

	PatternMatchState( const patterns::Pattern & pattern, const patterns::Alternative & alternative, const Node & startNode ) :
		variant( new MatchVariant( alternative ) ), startNode( startNode ) {}

	PatternMatchState( const PatternMatchState & state, const TransitionRef & transition, bool releaseParent ) :
		startNode( state.startNode ), variant( releaseParent ? state.releaseVariant() : new MatchVariant( *state.variant ) ), context( state.context ) {

		if ( &transition->start != &getCurrentNode() )
			throw std::logic_error("Illegal transition");

		context.addValue( getCurrentMatcher().variable, transition );

		variant->push_back( transition );
	}

	PatternMatchState( const PatternMatchState & state, const TransitionRef & transition, const Context & ctx, bool releaseParent ) :
		startNode( state.startNode ), variant( releaseParent ? state.releaseVariant() : new MatchVariant( *state.variant ) ), context( ctx ) {

		if ( &transition->start != &getCurrentNode() )
			throw std::logic_error("Illegal transition");

		context.addValue( getCurrentMatcher().variable, transition );

		variant->push_back( transition );
	}

	const Node & getCurrentNode() const {
		if ( variant->empty() )
			return startNode;

		return (*variant)[ variant->size() - 1 ]->end;
	}

	const matchers::Matcher & getCurrentMatcher() const {
		return variant->alternative.getMatcher( variant->size() );
	}

	const patterns::Alternative & getAlternative() const {
		return variant->alternative;
	}

	/**
	 * Освободить вариант сопоставления для дальнейшего использования.
	 * После вызова этого метода состояние становится невалидным и не может быть больше использовано.
	 * Вариант сопоставления освобождается из-под управления состояния и должен быть передан под управление другого контейнера.
	 */
	MatchVariant * releaseVariant() const {
		return variant.release();
	}

	/**
	 * Проверить является ли состояние завершенным.
	 */
	bool isComplete() const {
		return variant->alternative.getMatcherCount() == variant->size();
	}

private:
	mutable std::auto_ptr<MatchVariant> variant; // Вариант сопоставления
};

static void processCompoundPattern( const PatternMatchState & state, TransitionList & newTransitions ) {
	const Node & currentNode = state.getCurrentNode();

	if ( state.isComplete() ) { // Сопоставление завершено
		Match::AttributesMap attributes;

		state.context.addAttributes( attributes, state.getAlternative().getBindings() ); // Строим набор аттрибутов

		for ( uint i = 0; i < newTransitions.size(); ++ i ) {
			Match & match = static_cast<Match&>( *newTransitions[i] );

			if ( match.equals( state.getAlternative().getPattern(), state.startNode, currentNode, attributes ) ) {
				match.addVariant( state.releaseVariant() );
				return; // Если сопоставление уже было найдено
			}
		}

		newTransitions.push_back( new Match( state.startNode, currentNode, state.getAlternative().getPattern(), state.releaseVariant(), attributes ) ); // TODO Optimize

		return;
	}

	ChainList chains;

	if ( const AnnotationMatcher * curMatcher = dynamic_cast<const AnnotationMatcher *>( &state.getCurrentMatcher() ) ) {
		TransitionList nextTransitions = curMatcher->buildTransitions( currentNode, state.context );

		for ( uint i = 0, sz = nextTransitions.size(); i < sz; ++ i ) {
			PatternMatchState temp_state( state, nextTransitions[ i ], i == sz );
			processCompoundPattern( temp_state, newTransitions );
		}
	} else {
		const AnnotationChainMatcher & chainMatcher = static_cast<const AnnotationChainMatcher &>( state.getCurrentMatcher() );

		chains.clear();
		chainMatcher.buildChains( state.getCurrentNode(), state.context, chains );

		for ( uint i = 0, sz = chains.size(); i < sz; ++ i ) {
			PatternMatchState temp_state( state, chains[i].first, chains[i].second, i == sz - 1  );
			processCompoundPattern( temp_state, newTransitions);
		}
	}
}

PatternMatcher::PatternMatcher( const Pattern & pattern ) : AnnotationMatcher( PATTERN ), pattern( pattern ) {
}

PatternMatcher::~PatternMatcher() {
}

bool PatternMatcher::matchTransition(const Transition & transition, const Context & context) const {
	if ( transition.type != Transition::MATCH ) // Не совпадает тип перехода
		return false;

	const Match & match = static_cast<const Match&>( transition ); // Преобразуем переход

	if ( &match.getPattern() != &pattern )
		return false;

	return matchRestrictions( transition, context );
}

TransitionList PatternMatcher::buildTransitions( const text::Node & node, const Context & context ) const {
	TransitionList newTransitions;

	if ( node.text.isMatchesReady( pattern ) ) {
		foreach ( const TransitionRef & transition, node.transitions ) // Перебираем все ребра из текущей вершины
			if ( matchTransition( *transition, context ) )
				newTransitions.push_back( transition );
	} else {
		for ( uint i = 0; i < pattern.alternatives.size(); ++ i ) {
			PatternMatchState state( pattern, pattern.alternatives[i], node );
			processCompoundPattern( state, newTransitions );
		}
	}

	return newTransitions;
}

void PatternMatcher::buildTransitions( const text::Node & node, const patterns::Pattern & pattern, const patterns::Alternative & alt, const Context & context, TransitionList & results ) const {
	PatternMatchState state( pattern, alt, node );

	processCompoundPattern( state, results );
}

void PatternMatcher::buildTransitions( const text::Transition & transition, const patterns::Pattern & pattern, const patterns::Alternative & alt, const Context & context, TransitionList & results ) const {
	if ( const AnnotationMatcher * curMatcher = dynamic_cast<const AnnotationMatcher *>( &alt.getMatcher( 0 ) ) ) {
		if ( !curMatcher->matchTransition( transition, Context() ) )
			return;

		PatternMatchState s0( pattern, alt, transition.start );
		PatternMatchState state( s0, const_cast<text::Transition*>( &transition ), true );

		processCompoundPattern( state, results );
	} else {
		PatternMatchState s0( pattern, alt, transition.start );
		const AnnotationChainMatcher & chainMatcher = dynamic_cast<const AnnotationChainMatcher &>( alt.getMatcher( 0 ) );

		ChainList chains; // Список цепочек

		chainMatcher.buildChains( transition, s0.context, chains ); // Заполняем список цепочек

		// TODO Optimize: Необходимо отдельно рассмотреть случай пустой цепи
		for ( uint i = 0, sz = chains.size(); i < sz; ++ i ) {
			PatternMatchState state( s0, chains[i].first, chains[i].second, i == sz - 1 );
			processCompoundPattern( state, results );
		}
	}
}

void PatternMatcher::dump( std::ostream & out, const std::string & tabs ) const {
	out << "PatternMatcher{ name = " << pattern.name << ", variable = " << variable << ", restrictions = ";
	dumpRestrictions( out );
	out << " }";
}

bool PatternMatcher::equals( const Matcher & m ) const {
	if ( !Matcher::equals( m ) ) return false; // Разные сопоставители

	const PatternMatcher & pm = static_cast<const PatternMatcher &>( m );

	if ( &pm.pattern != &pattern ) return false; // Ссылаются на разные шаблоны

	return true;
}

} } } // namespace lspl::patterns::matchers
