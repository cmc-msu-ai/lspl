#include "../../base/BaseInternal.h"

#include "MatcherContainer.h"

#include "TokenMatcher.h"
#include "WordMatcher.h"
#include "PatternMatcher.h"
#include "LoopMatcher.h"

namespace lspl { namespace patterns { namespace matchers {

MatcherContainer::MatcherContainer() {
}

MatcherContainer::~MatcherContainer() {
}

void MatcherContainer::addMatcher( Matcher * matcher ) {
	matchers.push_back( matcher );
}

TokenMatcher & MatcherContainer::newTokenMatcher( const std::string & token ) {
	TokenMatcher * matcher = new TokenMatcher( token );

	addMatcher( matcher );

	return *matcher;
}

WordMatcher & MatcherContainer::newWordMatcher( const std::string & base, text::attributes::SpeechPart speechPart ) {
	WordMatcher * matcher = new WordMatcher( base, speechPart );

	addMatcher( matcher );

	return *matcher;
}

PatternMatcher & MatcherContainer::newPatternMatcher( const Pattern & pattern ) {
	PatternMatcher * matcher = new PatternMatcher( pattern );

	addMatcher( matcher );

	return *matcher;
}

LoopMatcher & MatcherContainer::newLoopMatcher( uint min, uint max ) {
	LoopMatcher * matcher = new LoopMatcher( min, max );

	addMatcher( matcher );

	return *matcher;
}

} } } // namespace lspl::patterns::matchers
