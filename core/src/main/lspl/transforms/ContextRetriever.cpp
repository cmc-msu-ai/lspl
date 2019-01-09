/*
 * ContextRetriever.cpp
 *
 *  Created on: Dec 6, 2009
 *      Author: alno
 */
#include "../base/BaseInternal.h"

#include "../text/Match.h"
#include "../text/Loop.h"
#include "../patterns/Alternative.h"
#include "../patterns/matchers/Context.h"
#include "../patterns/matchers/Matcher.h"
#include "../patterns/matchers/MatcherContainer.h"
#include "../patterns/matchers/LoopMatcher.h"

#include "ContextRetriever.h"

using lspl::patterns::matchers::Context;
using lspl::patterns::matchers::LoopMatcher;

using lspl::text::Loop;
using lspl::text::LoopIterationVariant;

namespace lspl { namespace transforms {

ContextRetriever::ContextRetriever() {
}

ContextRetriever::~ContextRetriever() {
}

Context ContextRetriever::apply( const text::MatchVariant & matchVariant ) const {
	Context ctx;
	appendToContext( ctx, matchVariant, matchVariant.alternative );
	return ctx;
}

void ContextRetriever::appendToContext( patterns::matchers::Context & ctx, const text::Transition & transition, const patterns::matchers::Matcher & matcher ) const {
	if ( const LoopMatcher * loopMatcher = dynamic_cast<const LoopMatcher *>( &matcher ) ) {
		const Loop & loop = dynamic_cast<const Loop &>( transition );

		for ( uint i = 0; i < loop.getIterationCount(); ++ i ) {
			const LoopIterationVariant & v = loop.getIteration( i )->getVariant( 0 );
			appendToContext( ctx, v, v.alternative );
		}
	} else {
		ctx.addValue( matcher.variable, &transition );
	}
}

void ContextRetriever::appendToContext( patterns::matchers::Context & ctx, const text::TransitionList & transitions, const patterns::matchers::MatcherContainer & matchers ) const {
	for ( uint i = 0; i < transitions.size(); ++ i )
		appendToContext( ctx, *transitions[i], matchers.getMatcher( i ) );
}

} } // namespace lspl::transforms
