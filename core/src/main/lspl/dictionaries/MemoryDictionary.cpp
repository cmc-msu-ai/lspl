/*
 * MemoryDictionary.cpp
 *
 *  Created on: Sep 27, 2008
 *      Author: alno
 */
#include "../base/BaseInternal.h"

#include "MemoryDictionary.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/member.hpp>

namespace lspl { namespace dictionaries {

class MemoryDictionary::WordListMap {
public:
	struct FirstWord {
		typedef std::string result_type;

		result_type operator()(const std::vector<std::string>& r) const {
			return r[0];
		}
	};

	typedef boost::multi_index::multi_index_container<
		std::vector<std::string>,
		boost::multi_index::indexed_by<
			boost::multi_index::hashed_non_unique<
				FirstWord
			>
		>
	> Map;
public:
	Map map;
};

MemoryDictionary::MemoryDictionary( const std::string & name ) :
	Dictionary( name ), map( new WordListMap() ) {
}

MemoryDictionary::~MemoryDictionary() {
}

bool MemoryDictionary::acceptsVector( const std::vector<std::string> & words ) const {
	if ( words.size() <= 0 )
		return false;

	std::pair<WordListMap::Map::const_iterator,WordListMap::Map::const_iterator> range = map->map.equal_range( words[0] );

	for ( WordListMap::Map::const_iterator i = range.first; i != range.second; ++ i )
		if ( *i == words )
			return true;

	return false;
}

void MemoryDictionary::add( const std::vector<std::string> & words ) {
	map->map.insert( words );
}

void MemoryDictionary::add( const std::string & w1 ) {
	std::vector<std::string> v;
	v.push_back( w1 );

	add( v );
}

void MemoryDictionary::add( const std::string & w1, const std::string & w2 ) {
	std::vector<std::string> v;
	v.push_back( w1 );
	v.push_back( w2 );

	add( v );
}

void MemoryDictionary::add( const std::string & w1, const std::string & w2, const std::string & w3 ) {
	std::vector<std::string> v;
	v.push_back( w1 );
	v.push_back( w2 );
	v.push_back( w3 );

	add( v );
}

} } // namespace lspl::dictionaries
