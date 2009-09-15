#include "../../base/BaseInternal.h"

#include "PatternMatcher.h"

#include "../Pattern.h"

#include "../../text/attributes/SpeechPart.h"
#include "../../text/Match.h"
#include "../../text/Text.h"
#include "../../text/Node.h"

#include <stdexcept>

using lspl::text::attributes::SpeechPart;

using lspl::text::Transition;
using lspl::text::TransitionRef;
using lspl::text::TransitionList;
using lspl::text::Match;

using lspl::text::Node;

namespace lspl { namespace patterns { namespace matchers {

struct PatternMatchState {
	const patterns::Pattern & pattern;
	const patterns::Alternative & alternative;

	const Node & startNode;

	TransitionList transitions;

	Context context;

	PatternMatchState( const patterns::Pattern & pattern, const patterns::Alternative & alternative, const Node & startNode ) :
		pattern( pattern ), alternative( alternative ), startNode( startNode ) {}

	PatternMatchState( const PatternMatchState & state, const TransitionRef & transition ) :
		pattern( state.pattern ), alternative( state.alternative ), startNode( state.startNode ), transitions( state.transitions ), context( state.context ) {

		if ( &transition->start != &getCurrentNode() )
			throw std::logic_error("Illegal transition");

		context.setVariable( getCurrentMatcher().variable, transition );

		transitions.push_back( transition );
	}

	PatternMatchState( const PatternMatchState & state, const TransitionRef & transition, const Context & ctx ) :
		pattern( state.pattern ), alternative( state.alternative ), startNode( state.startNode ), transitions( state.transitions ), context( ctx ) {

		if ( &transition->start != &getCurrentNode() )
			throw std::logic_error("Illegal transition");

		context.setVariable( getCurrentMatcher().variable, transition );

		transitions.push_back( transition );
	}

	const Node & getCurrentNode() const {
		if ( transitions.empty() )
			return startNode;

		return transitions[ transitions.size() - 1 ]->end;
	}

	const matchers::Matcher & getCurrentMatcher() const {
		return alternative.getMatcher( transitions.size() );
	}

	bool complete() const {
		return alternative.getMatcherCount() == transitions.size();
	}
};

static void processCompoundPattern( const PatternMatchState & state, TransitionList & newTransitions ) {
	const Node & currentNode = state.getCurrentNode();

	if ( state.complete() ) { // Сопоставление завершено
		base::Reference<Match> match( new Match( state.startNode, currentNode, state.pattern, state.alternative, state.context ) );

		for ( uint i = 0; i < newTransitions.size(); ++ i )
			if ( match->equals( *newTransitions[i] ) )
				return; // Если сопоставление уже было найдено

		newTransitions.push_back( match );
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
