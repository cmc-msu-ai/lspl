#include "../../base/BaseInternal.h"

#include "LoopMatcher.h"

#include "../Pattern.h"

#include "../../text/attributes/SpeechPart.h"
#include "../../text/Loop.h"
#include "../../text/Node.h"

#include <stdexcept>

using lspl::text::attributes::SpeechPart;

using lspl::text::Transition;
using lspl::text::TransitionList;
using lspl::text::TransitionRef;
using lspl::text::Loop;

using lspl::text::Node;

namespace lspl { namespace patterns { namespace matchers {

struct LoopMatchState {
	const LoopMatcher & matcher;

	const Node & startNode;

	const boost::ptr_vector<Matcher> & alternativeMatchers;

	Context context;

	TransitionList transitions;

	LoopMatchState( const LoopMatcher & matcher, const Node & startNode, const Context & context, const boost::ptr_vector<Matcher> & alternativeMatchers ) :
		matcher( matcher ), startNode( startNode ), alternativeMatchers( alternativeMatchers ), context( context ) {}

	LoopMatchState( const LoopMatchState & state, const TransitionRef & transition ) :
		matcher( state.matcher ), startNode( state.startNode ), alternativeMatchers( state.alternativeMatchers ), context( state.context ), transitions( state.transitions ) {

		if ( &transition->start != &getCurrentNode() )
			throw std::logic_error("Illegal transition");

		context.setVariable( getCurrentMatcher().variable, transition );

		transitions.push_back( transition );
	}

	LoopMatchState( const LoopMatchState & state, const TransitionRef & transition, const Context & ctx ) :
		matcher( state.matcher ), startNode( state.startNode ), alternativeMatchers( state.alternativeMatchers ), context( ctx ), transitions( state.transitions ) {

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
		return alternativeMatchers[ transitions.size() % alternativeMatchers.size() ];
	}

	bool complete() const {
		return transitions.size() % alternativeMatchers.size() == 0;
	}

	uint loops() const {
		return (uint)( transitions.size() / alternativeMatchers.size() );
	}
};

static void processLoop( const LoopMatchState & state, std::vector< std::pair<TransitionRef ,Context> > & results ) {
	const Node & currentNode = state.getCurrentNode();
	const Matcher & currentMatcher = state.getCurrentMatcher();

	if ( state.complete() ) {
		if ( state.loops() > 0 && state.loops() >= state.matcher.minLoops && ( state.loops() <= state.matcher.maxLoops || state.matcher.maxLoops == 0 ) ) {
			results.push_back( std::make_pair( new Loop( state.startNode, currentNode, state.transitions ), state.context ) );
		}

		if ( state.loops() >= state.matcher.maxLoops && state.matcher.maxLoops > 0 )
			return;
	}

	if ( const AnnotationMatcher * curMatcher = dynamic_cast<const AnnotationMatcher *>( &state.getCurrentMatcher() ) ) {
		TransitionList nextTransitions = curMatcher->buildTransitions( currentNode, state.context );

		for ( uint i = 0; i < nextTransitions.size(); ++ i )
			processLoop( LoopMatchState( state, nextTransitions[ i ] ), results );
	} else {
		const AnnotationChainMatcher & curMatcher = static_cast<const AnnotationChainMatcher &>( state.getCurrentMatcher() );
		ChainList chains;

		curMatcher.buildChains( state.getCurrentNode(), state.context, chains );

		for ( uint i = 0; i < chains.size(); ++ i )
			processLoop( LoopMatchState( state, chains[i].first, chains[i].second ), results );
	}
}

LoopMatcher::LoopMatcher() : AnnotationChainMatcher( LOOP ), minLoops( 0 ), maxLoops( 0 ) {
}

LoopMatcher::LoopMatcher( uint min, uint max ) : AnnotationChainMatcher( LOOP ), minLoops( min ), maxLoops( max ) {
}

LoopMatcher::~LoopMatcher() {
	for ( uint i = 0; i < alternatives.size(); ++i )
		if ( MatcherContainer * alternative = alternatives[i] )
			delete alternative;
}

void LoopMatcher::buildChains( const text::Node & node, const Context & context, ChainList & results ) const {
	if ( minLoops == 0 )
		results.push_back( std::make_pair( new Loop( node, node, TransitionList() ), context ) );

	for ( uint i = 0; i < alternatives.size(); ++ i )
		processLoop( LoopMatchState( *this, node, context, alternatives[i]->getMatchers() ), results );
}

void LoopMatcher::buildChains( const text::Transition & transition, const Context & context, ChainList & results ) const {
	if ( minLoops == 0 )
		results.push_back( std::make_pair( new Loop( transition.start, transition.start, TransitionList() ), context ) );

	for ( uint i = 0; i < alternatives.size(); ++ i ) {
		const LoopMatchState s0( *this, transition.start, context, alternatives[i]->getMatchers() );

		if ( const AnnotationMatcher * curMatcher = dynamic_cast<const AnnotationMatcher *>( &s0.getCurrentMatcher() ) ) {
			if ( !curMatcher->matchTransition( transition, Context() ) )
				return;

			processLoop( LoopMatchState( s0, const_cast<text::Transition *>( &transition ) ), results );
		} else {
			const AnnotationChainMatcher & chainMatcher = dynamic_cast<const AnnotationChainMatcher &>( s0.getCurrentMatcher() );

			ChainList chains; // Список цепочек

			chainMatcher.buildChains( transition, s0.context, chains ); // Заполняем список цепочек

			// TODO Необходимо отдельнор рассмотреть случайп пустой цепи
			for ( uint i = 0; i < chains.size(); ++ i )
				processLoop( LoopMatchState( s0, chains[i].first, chains[i].second ), results );
		}
	}
}

void LoopMatcher::dump( std::ostream & out, const std::string & tabs ) const {
	out << "LoopMatcher{ min = " << minLoops << ", max = " << maxLoops << ", matchers = [\n\t" << tabs;

	for ( uint j = 0; j < alternatives.size(); ++ j ) {
		if ( j != 0 )
			out << ",\n\t" << tabs;

		out << "[\n\t\t";

		const boost::ptr_vector<Matcher> & matchers = alternatives[j]->getMatchers();

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
