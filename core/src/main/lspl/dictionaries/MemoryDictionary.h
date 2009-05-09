/*
 * MemoryDictionary.h
 *
 *  Created on: Sep 27, 2008
 *      Author: alno
 */
#ifndef _LSPL_DICTIONARIES_MEMORYDICTIONARY_H_
#define _LSPL_DICTIONARIES_MEMORYDICTIONARY_H_

#include "../base/Base.h"
#include "Dictionary.h"

#include <boost/scoped_ptr.hpp>

namespace lspl { namespace dictionaries {

class LSPL_EXPORT MemoryDictionary : public Dictionary {
public:
	MemoryDictionary( const std::string & name );
	virtual ~MemoryDictionary();

	void add( const std::vector<std::string> & words );

	void add( const std::string & w1 );

	void add( const std::string & w1, const std::string & w2 );

	void add( const std::string & w1, const std::string & w2, const std::string & w3 );

protected:
	bool acceptsVector( const std::vector<std::string> & words ) const;
private:
	class WordListMap;
private:
	boost::scoped_ptr<WordListMap> map;
};

} } // namespace lspl::dictionaries

#endif//_LSPL_DICTIONARIES_MEMORYDICTIONARY_H_
