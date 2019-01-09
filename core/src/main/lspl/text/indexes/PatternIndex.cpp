/*
 * PatternIndex.cpp
 *
 *  Created on: Mar 18, 2009
 *      Author: alno
 */
#include "../../base/BaseInternal.h"
#include "PatternIndex.h"

#include "../../patterns/Pattern.h"
#include "../../patterns/matchers/Matcher.h"
#include "../Match.h"

#include <iostream>

using lspl::patterns::Pattern;

namespace lspl { namespace text { namespace indexes {

class PatternIndex::Impl {
public:
	typedef MatchList IndexValue;
	typedef std::map< uint, IndexValue > IndexMap;
public:
	IndexMap map;
};

class PatternIndex::IteratorImpl : public Index::Iterator {
public:
	IteratorImpl(Impl::IndexValue & value) : it( value.begin() ), end( value.end() ) {

	};

	virtual Transition * get() {
		return it->get();
	};

	virtual void increment() {
		++ it;
	};

	virtual bool finished() {
		return it == end;
	};

private:
	Impl::IndexValue::iterator it;
	Impl::IndexValue::iterator end;
};

PatternIndex::PatternIndex() : impl( new Impl() ) {
}

PatternIndex::~PatternIndex() {
}

Index::Iterator * PatternIndex::createIterator( const Pattern * patternPtr ) {
	Impl::IndexMap & map = impl->map;
	Impl::IndexMap::iterator i = map.find( patternPtr->id );

	if ( i == map.end() )
		return 0;

	return new IteratorImpl( i->second );
}

void PatternIndex::addPattern( const Pattern * patternPtr ) {
	Impl::IndexMap & map = impl->map;
	Impl::IndexMap::iterator i = map.find( patternPtr->id );

	if ( i != map.end() )
		throw std::logic_error("Pattern index already exists!");

	map.insert( std::make_pair( patternPtr->id, Impl::IndexValue() ) );
}

void PatternIndex::addAnnotation( const Pattern * patternPtr, Match * transitionPtr ) {
	Impl::IndexMap & map = impl->map;
	Impl::IndexMap::iterator i = map.find( patternPtr->id );

	if ( i == map.end() )
		throw std::logic_error("No pattern registered!");

	i->second.push_back( transitionPtr ); // Добавляем сопоставление в соответствующий индекс
}

const MatchList * PatternIndex::getMatches( const patterns::Pattern * patternPtr ) const {
	Impl::IndexMap & map = impl->map;
	Impl::IndexMap::const_iterator i = map.find( patternPtr->id );

	if ( i == map.end() ) {
		std::cerr << "No matches found for pattern " << patternPtr->name << "[" << patternPtr->id << "]";
		std::cerr.flush();
		return 0;
	}

	return &i->second;
}

} } } // namespace lspl::text::indexes
