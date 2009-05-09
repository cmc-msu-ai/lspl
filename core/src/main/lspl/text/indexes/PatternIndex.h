/*
 * PatternIndex.h
 *
 *  Created on: Mar 18, 2009
 *      Author: alno
 */

#ifndef _LSPL_TEXT_INDEXES_PATTERNINDEX_H_
#define _LSPL_TEXT_INDEXES_PATTERNINDEX_H_

#include "Index.h"

#include "../../patterns/Forward.h"

#include <boost/scoped_ptr.hpp>

namespace lspl { namespace text { namespace indexes {

/**
 * Индекс сопоставлений в тексте по типу сопоставленного шаблона.
 */
class LSPL_EXPORT PatternIndex : public Index {
public:
	PatternIndex();
	virtual ~PatternIndex();

	Iterator * createIterator( const patterns::Pattern * patternPtr );

	void addPattern( const patterns::Pattern * patternPtr );
	void addAnnotation( const patterns::Pattern * patternPtr, Match * transitionPtr );

	const MatchList * getMatches( const patterns::Pattern * patternPtr ) const;

private:
	class Impl;
	class IteratorImpl;

private:
	boost::scoped_ptr<Impl> impl;

};

} } } // namespace lspl::text::indexes

#endif /* _LSPL_TEXT_INDEXES_PATTERNINDEX_H_ */
