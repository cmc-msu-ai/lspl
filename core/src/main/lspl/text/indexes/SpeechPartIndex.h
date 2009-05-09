/*
 * SpeechPartIndex.h
 *
 *  Created on: Apr 15, 2009
 *      Author: alno
 */

#ifndef _LSPL_TEXT_INDEXES_SPEECHPARTINDEX_H_
#define _LSPL_TEXT_INDEXES_SPEECHPARTINDEX_H_

#include "Index.h"

#include "../attributes/SpeechPart.h"

#include <boost/scoped_ptr.hpp>

namespace lspl { namespace text { namespace indexes {

/**
 * Индекс слов в тексте по части речи.
 */
class LSPL_EXPORT SpeechPartIndex : public Index {
public:
	SpeechPartIndex();
	virtual ~SpeechPartIndex();

	Iterator * createIterator( attributes::SpeechPart speechPart );

	void addAnnotation( attributes::SpeechPart speechPart, markup::Word * transitionPtr );

	const markup::WordList * getWords( attributes::SpeechPart speechPart ) const;

private:
	class Impl;
	class IteratorImpl;

private:
	boost::scoped_ptr<Impl> impl;

};

} } } // namespace lspl::text::indexes

#endif /* _LSPL_TEXT_INDEXES_SPEECHPARTINDEX_H_ */
