/*
 * SpeechPartIndex.cpp
 *
 *  Created on: Apr 15, 2009
 *      Author: alno
 */

#include "../../base/BaseInternal.h"
#include "SpeechPartIndex.h"

#include "../markup/Word.h"

using lspl::text::attributes::SpeechPart;

using lspl::text::markup::Word;
using lspl::text::markup::WordList;

namespace lspl { namespace text { namespace indexes {

class SpeechPartIndex::Impl {
public:
	typedef WordList IndexValue;
	typedef std::vector< IndexValue > IndexMap;
public:
	IndexMap map;
};

class SpeechPartIndex::IteratorImpl : public Index::Iterator {
public:
	IteratorImpl(Impl::IndexValue & value) : it( value.begin() ), end( value.end() ) {
	}


	virtual Transition * get() {
		return it->get();
	}

	virtual void increment() {
		++ it;
	}

	virtual bool finished() {
		return it == end;
	}

private:
	Impl::IndexValue::iterator it;
	Impl::IndexValue::iterator end;
};

SpeechPartIndex::SpeechPartIndex() : impl( new Impl() ) {
	impl->map.resize( SpeechPart::COUNT );
}

SpeechPartIndex::~SpeechPartIndex() {
}

Index::Iterator * SpeechPartIndex::createIterator( SpeechPart speechPart ) {
	Impl::IndexMap & map = impl->map;
	uint speechPartId = speechPart.id;

	if ( map.size() <= speechPartId )
		return 0;

	return new IteratorImpl( map[ speechPartId ] );
}

void SpeechPartIndex::addAnnotation( SpeechPart speechPart, Word * transitionPtr ) {
	Impl::IndexMap & map = impl->map;
	uint speechPartId = speechPart.id;

	if ( map.size() <= speechPartId ) // Расширяем индекс в случае необходимости
		map.resize( speechPartId + 1 );

	map.at( speechPartId ).push_back( transitionPtr ); // Добавляем сопоставление в соответствующий индекс
}

const WordList * SpeechPartIndex::getWords( SpeechPart speechPart ) const {
	Impl::IndexMap & map = impl->map;
	uint speechPartId = speechPart.id;

	if ( map.size() <= speechPartId )
		return 0;

	return &map[ speechPartId ];
}

} } } // namespace lspl::text::indexes
