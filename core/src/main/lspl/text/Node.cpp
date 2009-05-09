#include "../base/BaseInternal.h"

#include <string>
#include <iostream>

#include "Node.h"
#include "Transition.h"

LSPL_REFCOUNT_CLASS( lspl::text::Node )

namespace lspl { namespace text {

Node::Node( uint index, uint startOffset, uint endOffset, const Text & text ) :
	index( index ), startOffset( startOffset ), endOffset( endOffset ), text( text ) {
}

Node::~Node() {
}

void Node::dump(std::ostream & out, std::string tabs) const {
	out << "Node{ index = " << index << ", startOffset = " << startOffset << ", endOffset = " << endOffset << " transitions = [\n\t" << tabs;

	bool first = true;

	foreach ( const TransitionRef & tr, transitions ) {
		if ( first ) {
			first = false;
		} else {
			out << ",\n\t" << tabs;
		}

		tr->dump(out, tabs + "\t");
	}

	out <<"\n" << tabs << "] }";
}

} } // namespace lspl
