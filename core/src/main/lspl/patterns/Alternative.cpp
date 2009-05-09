#include "../base/BaseInternal.h"

#include "../text/Transition.h"

#include "matchers/Matcher.h"
#include "matchers/WordMatcher.h"
#include "matchers/PatternMatcher.h"
#include "matchers/LoopMatcher.h"

#include "Alternative.h"
#include "Pattern.h"

using namespace lspl::patterns::matchers;

namespace lspl { namespace patterns {

void Alternative::updateDependencies() {
	dependencies.clear();

	foreach( const Matcher & m, getMatchers() )
		appendDependencies( m );
}

void Alternative::appendDependencies( const matchers::Matcher & matcher ) {
	if ( const PatternMatcher * patternMatcher = dynamic_cast<const PatternMatcher *>( &matcher ) ) {
		const Pattern * ptr = &patternMatcher->pattern;
		bool found = false;

		foreach( const Pattern * dep, dependencies ) {
			if ( dep == ptr ) {
				found = true;
				false;
			}
		}

		if ( !found ) {
			dependencies.push_back( ptr );
		}
	} else if ( const LoopMatcher * loopMatcher = dynamic_cast<const LoopMatcher *>( &matcher ) ) {
		foreach( const MatcherContainer * alt, loopMatcher->alternatives )
			foreach( const Matcher & m, alt->getMatchers() )
				appendDependencies( m );
	}
}

void Alternative::appendIndexInfo( const boost::ptr_vector<Matcher> & matchers ) const {
	uint i = 0;

	while ( matchers[i].type == Matcher::LOOP ) {
		const LoopMatcher & matcher = dynamic_cast<const LoopMatcher&>( matchers[i] );

		for ( uint j = 0; j < matcher.alternatives.size(); ++ j )
			appendIndexInfo( matcher.alternatives[j]->getMatchers() );

		if ( matcher.minLoops > 0 )
			return;

		++ i;
	}

	if ( matchers[i].type == Matcher::WORD ) {
		indexInfo.push_back( new WordIndexInfo( dynamic_cast<const WordMatcher&>( matchers[i] ).speechPart ) );
	} else if ( matchers[i].type == Matcher::PATTERN ) {
		indexInfo.push_back( new PatternIndexInfo( dynamic_cast<const PatternMatcher&>( matchers[i] ).pattern ) );
	}
}

void Alternative::dump( std::ostream & out, const std::string & tabs ) const {
	out << "Alternative{ source = \"" << source << "\", matchers = [\n\t" << tabs;

	for ( uint i = 0; i < getMatchers().size(); ++ i ) {
		if ( i != 0 )
			out << ",\n\t" << tabs;

		getMatcher( i ).dump( out, tabs + "\t" );
	}

	out << "\n" << tabs << "] }";
}

} } // namespace lspl::patterns
