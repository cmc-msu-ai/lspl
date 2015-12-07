#ifndef _LSPL_TEXT_ATTRIBUTES_INDEXEDENTRYMANAGER_H_
#define _LSPL_TEXT_ATTRIBUTES_INDEXEDENTRYMANAGER_H_

#include "../../base/Base.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/shared_ptr.hpp>

namespace lspl { namespace text { namespace attributes {

class IndexedEntryManager {
public:
	class Entry {
	public:
		Entry( uint id, const std::string & abbrevation, const std::string & name, const std::string & title ) :
			id( id ), abbrevation( abbrevation ), name( name ), title( title ) {		
		}
		
		const uint id;
		const std::string abbrevation;
		const std::string name;
		const std::string title;
	};
	
	typedef boost::multi_index::multi_index_container<
		boost::shared_ptr< Entry >,
		boost::multi_index::indexed_by<
			boost::multi_index::hashed_unique< 
				boost::multi_index::member< Entry, const std::string, &Entry::abbrevation >
			>,
			boost::multi_index::hashed_unique< 
				boost::multi_index::member< Entry, const std::string, &Entry::name >
			>,
			boost::multi_index::random_access<
			>
		>
	> EntryMap;
	
	typedef EntryMap::iterator Iterator;
public:	
	uint add( uint id, const std::string & abbrevation, const std::string & name, const std::string & title ) {		
		map.insert( boost::shared_ptr< Entry >( new Entry( id, abbrevation, name, title ) ) );
		return id;
	}
	
	Entry & at( uint index ) {
		return *map.get<2>().at( index );
	}
public:
	EntryMap map;
};

} } }

#endif//_LSPL_TEXT_ATTRIBUTES_INDEXEDENTRYMANAGER_H_
