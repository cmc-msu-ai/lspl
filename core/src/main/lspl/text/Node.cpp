#include "../base/BaseInternal.h"

#include <string>
#include <iostream>

#include "Node.h"
#include "Transition.h"
#include "Text.h"

#include "Match.h"
#include "markup/Word.h"
#include "markup/Token.h"

#include "../patterns/matchers/Matcher.h"

LSPL_REFCOUNT_CLASS( lspl::text::Node )

namespace lspl { namespace text {

uint Node::aliveObjectsCount = 0;

Node::Node( uint index, uint startOffset, uint endOffset, const Text & text ) :
	index( index ), startOffset( startOffset ), endOffset( endOffset ), text( text ), tokenCount( 0 ), wordCount( 0 ), matchCount( 0 ) {
    ++ aliveObjectsCount;
}

Node::~Node() {
    -- aliveObjectsCount;
}

std::string Node::getRangeString() const {
	return text.getContent().substr( startOffset, endOffset - startOffset );
}

void Node::dump(std::ostream & out, std::string tabs) const {
	out << "Node{ index = " << index << ", startOffset = " << startOffset << ", endOffset = " << endOffset << " transitions = [\n\t" << tabs;

	bool first = true;

	BOOST_FOREACH( const TransitionRef & tr, transitions ) {
		if ( first ) {
			first = false;
		} else {
			out << ",\n\t" << tabs;
		}

		tr->dump(out, tabs + "\t");
	}

	out <<"\n" << tabs << "] }";
}

void Node::addTokenTransition( const markup::TokenRef & token ) {
	if ( wordCount > 0 )
		throw std::logic_error( "Tokens should be added before words" );

	if ( matchCount > 0 )
		throw std::logic_error( "Tokens should be added before matches" );

	transitions.push_back( token );

	++ tokenCount;
}

void Node::addWordTransition( const markup::WordRef & word ) {
	if ( matchCount > 0 )
		throw std::logic_error( "Words should be added before matches" );

	transitions.push_back( word );

	++ wordCount;
}

void Node::addMatchTransition( const MatchRef & match ) {
	transitions.push_back( match );

	++ matchCount;
}

} } // namespace lspl
