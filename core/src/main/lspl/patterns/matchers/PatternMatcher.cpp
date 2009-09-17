#include "../../base/BaseInternal.h"

#include "PatternMatcher.h"

#include "../Pattern.h"

#include "../../text/attributes/SpeechPart.h"
#include "../../text/Match.h"
#include "../../text/Text.h"
#include "../../text/Node.h"

#include <stdexcept>

using lspl::text::attributes::SpeechPart;

using lspl::text::Node;
using lspl::text::Transition;
using lspl::text::TransitionRef;
using lspl::text::TransitionList;
using lspl::text::Match;
using lspl::text::MatchVariant;

namespace lspl { namespace patterns { namespace matchers {

struct PatternMatchState {
	const patterns::Pattern & pattern;

	const Node & startNode;

	MatchVariant variant; // Вариант сопоставления

	Context context;

	PatternMatchState( const patterns::Pattern & pattern, const patterns::Alternative & alternative, const Node & startNode ) :
		pattern( pattern ), variant( alternative ), startNode( startNode ) {}

	PatternMatchState( const PatternMatchState & state, const TransitionRef & transition ) :
		pattern( state.pattern ), startNode( state.startNode ), variant( state.variant ), context( state.context ) {

		if ( &transition->start != &getCurrentNode() )
			throw std::logic_error("Illegal transition");

		context.setVariable( getCurrentMatcher().variable, transition );

		variant.push_back( transition );
	}

	PatternMatchState( const PatternMatchState & state, const TransitionRef & transition, const Context & ctx ) :
		pattern( state.pattern ), startNode( state.startNode ), variant( state.variant ), context( ctx ) {

		if ( &transition->start != &getCurrentNode() )
			throw std::logic_error("Illegal transition");

		context.setVariable( getCurrentMatcher().variable, transition );

		variant.push_back( transition );
	}

	const Node & getCurrentNode() const {
		if ( variant.empty() )
			return startNode;

		return variant[ variant.size() - 1 ]->end;
	}

	const matchers::Matcher & getCurrentMatcher() const {
		return variant.alternative.getMatcher( variant.size() );
	}

	bool complete() const {
		return variant.alternative.getMatcherCount() == variant.size();
	}
};

static void processCompoundPattern( const PatternMatchState & state, TransitionList & newTransitions ) {
	const Node & currentNode = state.getCurrentNode();

	if ( state.complete() ) { // Сопоставление завершено
		Match::AttributesMap attributes;

		state.context.addAttributes( attributes, state.variant.alternative.getBindings() ); // Строим набор аттрибутов

		for ( uint i = 0; i < newTransitions.size(); ++ i ) {
			Match & match = static_cast<Match&>( *newTransitions[i] );

			if ( match.equals( state.pattern, state.startNode, currentNode, attributes ) ) {
				match.addVariant( new MatchVariant( state.variant ) ); // TODO Optimize
				return; // Если сопоставление уже было найдено
			}
		}

		newTransitions.push_back( new Match( state.startNode, currentNode, state.pattern, new MatchVariant( state.variant ), attributes ) ); // TODO Optimize

		return;
	}

	ChainList chains;

	if ( const AnnotationMatcher * curMatcher = dynamic_cast<const AnnotationMatcher *>( &state.getCurrentMatcher() ) ) {
		TransitionList nextTransitions = curMatcher->buildTransitions( currentNode, state.context );

		for ( uint i = 0; i < nextTransitions.size(); ++ i )
			processCompoundPattern( PatternMatchState( state, nextTransitions[ i ] ), newTransitions );

	} else {
		const AnnotationChainMatcher & chainMatcher = static_cast<const AnnotationChainMatcher &>( state.getCurrentMatcher() );

		chains.clear();
		chainMatcher.buildChains( state.getCurrentNode(), state.context, chains );

		for ( uint i = 0; i < chains.size(); ++ i )
			processCompoundPattern( PatternMatchState( state, chains[i].first, chains[i].second ), newTransitions );
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
		for ( uint i = 0; i < node.transitions.size(); ++ i )
			if ( matchTransition( *node.transitions[i], context ) )
				newTransitions.push_back( node.transitions[i] );
	} else {
		for ( uint i = 0; i < pattern.alternatives.size(); ++ i )
			processCompoundPattern( PatternMatchState( pattern, pattern.alternatives[i], node ), newTransitions );
	}

	return newTransitions;
}

void PatternMatcher::buildTransitions( const text::Node & node, const patterns::Pattern & pattern, const patterns::Alternative & alt, const Context & context, TransitionList & results ) const {
	processCompoundPattern( PatternMatchState( pattern, alt, node ), results );
}

void PatternMatcher::buildTransitions( const text::Transition & transition, const patterns::Pattern & pattern, const patterns::Alternative & alt, const Context & context, TransitionList & results ) const {
	const PatternMatchState s0( pattern, alt, transition.start );

	if ( const AnnotationMatcher * curMatcher = dynamic_cast<const AnnotationMatcher *>( &s0.getCurrentMatcher() ) ) {
		if ( !curMatcher->matchTransition( transition, Context() ) )
			return;

		processCompoundPattern( PatternMatchState( s0, const_cast<text::Transition*>( &transition ) ), results );
	} else {
		const AnnotationChainMatcher & chainMatcher = dynamic_cast<const AnnotationChainMatcher &>( s0.getCurrentMatcher() );

		ChainList chains; // Список цепочек

		chainMatcher.buildChains( transition, s0.context, chains ); // Заполняем список цепочек

		// TODO Необходимо отдельнор рассмотреть случайп пустой цепи
		for ( uint i = 0; i < chains.size(); ++ i )
			processCompoundPattern( PatternMatchState( s0, chains[i].first, chains[i].second ), results );
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
