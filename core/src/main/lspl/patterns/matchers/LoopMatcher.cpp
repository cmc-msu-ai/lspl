#include "../../base/BaseInternal.h"

#include "LoopMatcher.h"

#include "../Pattern.h"

#include "../../text/attributes/SpeechPart.h"
#include "../../text/Loop.h"
#include "../../text/Node.h"

#include <stdexcept>

#include <boost/noncopyable.hpp>

using lspl::text::attributes::SpeechPart;

using lspl::text::Transition;
using lspl::text::TransitionList;
using lspl::text::TransitionRef;
using lspl::text::Loop;
using lspl::text::LoopIteration;
using lspl::text::LoopIterationRef;
using lspl::text::LoopIterationList;
using lspl::text::LoopIterationVariant;

using lspl::text::Node;

namespace lspl { namespace patterns { namespace matchers {

struct LoopIterationMatchState : public boost::noncopyable {
	const LoopMatcher & matcher;

	const Node & startNode;

	Context context;

	LoopIterationMatchState( const LoopMatcher & matcher, const Node & startNode, const Context & context, const MatcherContainer & alternative ) :
		matcher( matcher ), startNode( startNode ), variant( new LoopIterationVariant( alternative ) ), context( context ) {}

	LoopIterationMatchState( const LoopIterationMatchState & state, const TransitionRef & transition ) :
		matcher( state.matcher ), startNode( state.startNode ), variant( new LoopIterationVariant( *state.variant ) ), context( state.context ) {

		if ( &transition->start != &getCurrentNode() )
			throw std::logic_error("Illegal transition");

		context.addValue( getCurrentMatcher().variable, transition );

		variant->push_back( transition );
	}

	LoopIterationMatchState( const LoopIterationMatchState & state, const TransitionRef & transition, const Context & ctx ) :
		matcher( state.matcher ), startNode( state.startNode ), variant( new LoopIterationVariant( *state.variant ) ), context( ctx ) {

		if ( &transition->start != &getCurrentNode() )
			throw std::logic_error("Illegal transition");

		context.addValue( getCurrentMatcher().variable, transition );

		variant->push_back( transition );
	}

	const Node & getCurrentNode() const {
		if ( variant->empty() )
			return startNode;

		return variant->at( variant->size() - 1 )->end;
	}

	const matchers::Matcher & getCurrentMatcher() const {
		return variant->alternative.getMatcher( variant->size() );
	}

	bool isComplete() const {
		return variant->size() == variant->alternative.getMatcherCount();
	}

	/**
	 * Освободить вариант итерации для дальнейшего использования.
	 * После вызова этого метода состояние становится невалидным и не может быть больше использовано.
	 * Вариант итерации освобождается из-под управления состояния и должен быть передан под управление другого контейнера.
	 */
	LoopIterationVariant * releaseVariant() const {
		return variant.release();
	}

private:

	mutable std::auto_ptr<LoopIterationVariant> variant; // Вариант сопоставления

};

struct LoopMatchState : public boost::noncopyable {
	const LoopMatcher & matcher;

	const Node & startNode;

	Context context;

	LoopIterationList iterations;

	LoopMatchState( const LoopMatcher & matcher, const Node & startNode, const Context & context ) :
		matcher( matcher ), startNode( startNode ), context( context ) {}

	LoopMatchState( const LoopMatchState & state, const LoopIterationRef & iteration, const Context & ctx ) :
		matcher( state.matcher ), startNode( state.startNode ), context( ctx ), iterations( state.iterations ) {

		iterations.push_back( iteration );
	}

	const Node & getCurrentNode() const {
		if ( iterations.empty() )
			return startNode;

		return iterations[ iterations.size() - 1 ]->end;
	}

	uint loops() const {
		return (uint) iterations.size();
	}

};

static void processLoopIteration( const LoopIterationMatchState & state, std::vector< std::pair<LoopIterationRef,Context> > & results ) {
	const Node & currentNode = state.getCurrentNode();

	if ( state.isComplete() ) {
		LoopIterationVariant * v = state.releaseVariant();

		for ( uint i = 0; i < results.size(); ++ i ) {
			std::pair<LoopIterationRef,Context> & p = results[i];

			if ( &p.first->end == &currentNode && p.second == state.context ) {
				p.first->addVariant( v );
				return;
			}
		}


		results.push_back( std::make_pair( new LoopIteration( state.startNode, currentNode, v ), state.context ) );

		return;
	}

	const Matcher & currentMatcher = state.getCurrentMatcher();

	if ( const AnnotationMatcher * curMatcher = dynamic_cast<const AnnotationMatcher *>( &state.getCurrentMatcher() ) ) {
		TransitionList nextTransitions = curMatcher->buildTransitions( currentNode, state.context );

		for ( uint i = 0; i < nextTransitions.size(); ++ i ) {
			LoopIterationMatchState loopIterationMatchState( state, nextTransitions[ i ] );
			processLoopIteration( loopIterationMatchState, results );
		}
	} else {
		const AnnotationChainMatcher & chainMatcher = static_cast<const AnnotationChainMatcher &>( state.getCurrentMatcher() );
		ChainList chains;

		chainMatcher.buildChains( state.getCurrentNode(), state.context, chains );

		for ( uint i = 0; i < chains.size(); ++ i ) {
			LoopIterationMatchState loopIterationMatchState( state, chains[i].first, chains[i].second );
			processLoopIteration( loopIterationMatchState, results );
		}
	}
}

static void processLoop( const LoopMatchState & state, ChainList & results );

static void processLoop( const LoopMatchState & state, std::vector< std::pair<LoopIterationRef,Context> > & iterations, ChainList & results ) {
	for ( uint i = 0; i < iterations.size(); ++ i ) {
		std::pair<LoopIterationRef,Context> & p = iterations[ i ];
		LoopMatchState loopMatchState( state, p.first, p.second );

		processLoop( loopMatchState, results );
	}
}

static void processLoop( const LoopMatchState & state, ChainList & results ) {
	uint loops = state.loops(); // Количество произведенных повторений

	if ( loops > 0 && loops >= state.matcher.minLoops && ( loops <= state.matcher.maxLoops || state.matcher.maxLoops == 0 ) ) { // Если число итераций приемлимо
		Loop * loop = new Loop( state.startNode, state.getCurrentNode(), loops );

		loop->iterations = state.iterations;

		results.push_back( std::make_pair( loop, state.context ) );
	}

	if ( loops >= state.matcher.maxLoops && state.matcher.maxLoops > 0 ) // Выходим, если достигли максимального числа итераций
		return;

	std::vector< std::pair<LoopIterationRef,Context> > iterations;

	for ( uint i = 0; i < state.matcher.alternatives.size(); ++ i ) {
		LoopIterationMatchState loopIterationMatchState( state.matcher, state.getCurrentNode(), state.context, state.matcher.alternatives[ i ] );
		processLoopIteration( loopIterationMatchState, iterations );
	}

	processLoop( state, iterations, results );
}

LoopMatcher::LoopMatcher() : AnnotationChainMatcher( LOOP ), minLoops( 0 ), maxLoops( 0 ) {
}

LoopMatcher::LoopMatcher( uint min, uint max ) : AnnotationChainMatcher( LOOP ), minLoops( min ), maxLoops( max ) {
}

LoopMatcher::~LoopMatcher() {
}

void LoopMatcher::buildChains( const text::Node & node, const Context & context, ChainList & results ) const {
	if ( minLoops == 0 )
		results.push_back( std::make_pair( new Loop( node, node, 0 ), context ) );

	LoopMatchState loopMatchState( *this, node, context );
	processLoop( loopMatchState, results );
}

void LoopMatcher::buildChains( const text::Transition & transition, const Context & context, ChainList & results ) const {
	if ( minLoops == 0 )
		results.push_back( std::make_pair( new Loop( transition.start, transition.start, 0 ), context ) );

	const LoopMatchState state( *this, transition.start, context );

	std::vector< std::pair<LoopIterationRef,Context> > iterations;

	for ( uint i = 0; i < alternatives.size(); ++ i ) {
		LoopIterationMatchState s0( state.matcher, state.getCurrentNode(), state.context, state.matcher.alternatives[ i ] );

		if ( const AnnotationMatcher * curMatcher = dynamic_cast<const AnnotationMatcher *>( &s0.getCurrentMatcher() ) ) {
			if ( curMatcher->matchTransition( transition, Context() ) ) {
				LoopIterationMatchState loopIterationMatchState( s0, const_cast<text::Transition *>( &transition ) );
				processLoopIteration( loopIterationMatchState, iterations );
			}
		} else {
			const AnnotationChainMatcher & chainMatcher = dynamic_cast<const AnnotationChainMatcher &>( s0.getCurrentMatcher() );

			ChainList chains; // Список цепочек

			chainMatcher.buildChains( transition, s0.context, chains ); // Заполняем список цепочек

			// TODO Необходимо отдельнор рассмотреть случайп пустой цепи
			for ( uint i = 0; i < chains.size(); ++ i ) {
				LoopIterationMatchState loopIterationMatchState( s0, chains[i].first, chains[i].second );
				processLoopIteration( loopIterationMatchState, iterations );
			}
		}
	}

	processLoop( state, iterations, results );
}

void LoopMatcher::dump( std::ostream & out, const std::string & tabs ) const {
	out << "LoopMatcher{ min = " << minLoops << ", max = " << maxLoops << ", matchers = [\n\t" << tabs;

	for ( uint j = 0; j < alternatives.size(); ++ j ) {
		if ( j != 0 )
			out << ",\n\t" << tabs;

		out << "[\n\t\t";

		const boost::ptr_vector<Matcher> & matchers = alternatives[j].getMatchers();

		for ( uint i = 0; i < matchers.size(); ++ i ) {
			if ( i != 0 )
				out << ",\n\t\t" << tabs;

			matchers[i].dump( out, tabs + "\t\t" );
		}

		out << "]";
	}

	out << "\n" << tabs << "] }";
}

} } } // namespace lspl::patterns::matchers
