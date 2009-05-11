#include "../base/BaseInternal.h"

#include "Loop.h"

#include "../text/Node.h"

#include "../text/attributes/AttributeValue.h"

using namespace lspl::text::attributes;

namespace lspl { namespace text {

Loop::Loop( const text::Node & start, const text::Node & end, const TransitionList & transitions ) :
	Transition( LOOP, start, end ), transitions( transitions ) {
}

Loop::~Loop() {
}

void Loop::dump( std::ostream & out, std::string tabs ) const {
	out << "Loop{ start = " << start.index
			<< ", end = " << end.index << " }";
}

} }
