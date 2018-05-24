#include "../base/BaseInternal.h"

#include "../text/Transition.h"

#include "matchers/Matcher.h"
#include "matchers/WordMatcher.h"
#include "matchers/PatternMatcher.h"
#include "matchers/LoopMatcher.h"

#include "../transforms/Transform.h"

#include "Alternative.h"
#include "Pattern.h"

using namespace lspl::patterns::matchers;

namespace lspl { namespace patterns {

Alternative::Alternative( const std::string & source, const std::string & transformSource ) :
	source( source ), transformSource( transformSource ) {
}

Alternative::~Alternative() {
}

void Alternative::setTransform( std::unique_ptr<transforms::Transform> t ) {
	transform = std::move( t );
}

bool Alternative::equals( const Alternative & alt ) const {
	if ( alt.getMatcherCount() != getMatcherCount() ) return false; // Различное количество сопоставителей
	if ( alt.bindings.size() != bindings.size() ) return false; // Различное количество связываний

	for ( uint i = 0, l = getMatcherCount(); i < l; ++ i ) // Сравниваем все сопоставители
		if ( !alt.getMatcher( i ).equals( getMatcher( i ) ) )
			return false;

	for ( BindingMap::const_iterator it = bindings.begin(); it != bindings.end(); ++ it ) { // Перебираем все связывания
		BindingMap::const_iterator ait = alt.bindings.find( it->first ); // Находим соответствующие

		if ( ait == alt.bindings.end() ) return false; // Если соответствующего нет - альтернативы не равны
		if ( !it->second->equals( *ait->second ) ) return false; // Если соответствующее отличается - альтернативы не равны
	}

	return true;
}

void Alternative::updateDependencies() {
	dependencies.clear();

	for( boost::ptr_vector<Matcher>::const_iterator it = getMatchers().begin(); it != getMatchers().end(); ++ it )
		appendDependencies( *it );
}

void Alternative::appendDependencies( const matchers::Matcher & matcher ) {
	if ( const PatternMatcher * patternMatcher = dynamic_cast<const PatternMatcher *>( &matcher ) ) {
		const Pattern * ptr = &patternMatcher->pattern;
		bool found = false;

		for( const Pattern * dep : dependencies ) {
			if ( dep == ptr ) {
				found = true;
				false;
			}
		}

		if ( !found ) {
			dependencies.push_back( ptr );
		}
	} else if ( const LoopMatcher * loopMatcher = dynamic_cast<const LoopMatcher *>( &matcher ) ) {
		for( boost::ptr_vector<MatcherContainer>::const_iterator altIt = loopMatcher->alternatives.begin(); altIt != loopMatcher->alternatives.end(); ++ altIt ) {
			const MatcherContainer & alt = *altIt;
			for( boost::ptr_vector<Matcher>::const_iterator it = alt.getMatchers().begin(); it != alt.getMatchers().end(); ++ it )
				appendDependencies( *it );
		}
	}
}

void Alternative::appendStartMatchers( const boost::ptr_vector<Matcher> & matchers ) const {
	uint i = 0;

	while ( i < matchers.size() && matchers[i].type == Matcher::LOOP ) {
		const LoopMatcher & matcher = dynamic_cast<const LoopMatcher&>( matchers[i] );

		for ( uint j = 0; j < matcher.alternatives.size(); ++ j )
			appendStartMatchers( matcher.alternatives[j].getMatchers() );

		if ( matcher.minLoops > 0 )
			return;

		++ i;
	}

	if ( i >= matchers.size() )
		return;

	startMatchers.push_back( &matchers[i] );
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
