#include "base/BaseInternal.h"

#include "Namespace.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/member.hpp>

#include <map>
#include <string>

using lspl::patterns::Pattern;
using lspl::dictionaries::Dictionary;

LSPL_REFCOUNT_CLASS( lspl::Namespace );

namespace lspl {

class Namespace::PatternMap {
public:
	typedef boost::multi_index::multi_index_container<
		base::Reference< Pattern >,
		boost::multi_index::indexed_by<
			boost::multi_index::hashed_unique<
				boost::multi_index::member< Pattern, const std::string, &Pattern::name >
			>,
			boost::multi_index::random_access<
			>
		>
	> Map;
public:
	Map map;
};

class Namespace::DictionaryMap {
public:
	typedef boost::multi_index::multi_index_container<
		base::Reference< Dictionary >,
		boost::multi_index::indexed_by<
			boost::multi_index::hashed_unique<
				boost::multi_index::member< Dictionary, const std::string, &Dictionary::name >
			>,
			boost::multi_index::random_access<
			>
		>
	> Map;
public:
	Map map;
};

Namespace::Namespace() :
	patternMap( new PatternMap() ), dictionaryMap( new DictionaryMap() ) {
}

Namespace::~Namespace() {
}

uint Namespace::getPatternCount() const {
	return patternMap->map.size();
}

base::Reference< Pattern > Namespace::getPatternByIndex( uint index ) const {
	return patternMap->map.get<1>().at( index );
}

base::Reference< Pattern > Namespace::getPatternByName( const std::string & name ) const {
	PatternMap::Map::iterator i = patternMap->map.find( name );

	if ( i == patternMap->map.end() )
		return base::Reference< Pattern >();

	return *i;
}

base::Reference< Pattern > Namespace::addPattern( const base::Reference< Pattern > & pattern ) {
	patternMap->map.insert( pattern );

	return pattern;
}

uint Namespace::getDictionaryCount() const {
	return dictionaryMap->map.size();
}

base::Reference< Dictionary > Namespace::getDictionaryByIndex( uint index ) const {
	return dictionaryMap->map.get<1>().at( index );
}

base::Reference< Dictionary > Namespace::getDictionaryByName( const std::string & name ) const {
	DictionaryMap::Map::iterator i = dictionaryMap->map.find( name );

	if ( i == dictionaryMap->map.end() )
		return base::Reference< Dictionary >();

	return *i;
}

base::Reference< Dictionary > Namespace::addDictionary( const base::Reference< Dictionary > & dict ) {
	dictionaryMap->map.insert( dict );

	return dict;
}

} // namespace lspl
