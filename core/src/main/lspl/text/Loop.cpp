#include "../base/BaseInternal.h"

#include "Loop.h"

#include "../text/Node.h"
#include "../text/attributes/AttributeValue.h"
#include "../patterns/matchers/Matcher.h"

using namespace lspl::text::attributes;

LSPL_REFCOUNT_CLASS( lspl::text::LoopIteration );

namespace lspl { namespace text {

LoopIterationVariant::LoopIterationVariant( const patterns::matchers::MatcherContainer & alternative ) :
	TransitionList(), alternative( alternative ) {
}

LoopIterationVariant::LoopIterationVariant( const LoopIterationVariant & variant ) :
	TransitionList( variant ), alternative( variant.alternative ) {
}

LoopIterationVariant::~LoopIterationVariant() {
}

void LoopIteration::dump( std::ostream & out, std::string tabs ) const {
	out << "LoopIteration{ start = " << start.index
			<< ", end = " << end.index << " }";
}

Loop::Loop( const text::Node & start, const text::Node & end, uint repeatCount ) :
	Transition( LOOP, start, end ), repeatCount( repeatCount ) {
}

Loop::~Loop() {
}

void Loop::dump( std::ostream & out, std::string tabs ) const {
	out << "Loop{ start = " << start.index
			<< ", end = " << end.index << " }";
}

} }
