#include "base/BaseInternal.h"

#include "Namespace.h"

#include "patterns/matchers/Matcher.h"
#include "patterns/Pattern.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/member.hpp>

#include <map>
#include <string>

using lspl::patterns::Pattern;
using lspl::patterns::PatternRef;
using lspl::dictionaries::Dictionary;
using lspl::dictionaries::DictionaryRef;

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

Namespace::Namespace(const NamespaceRef & parentNamespace) :
	patternMap( new PatternMap() ), dictionaryMap( new DictionaryMap() ) {
	parents.push_back( parentNamespace );
}

Namespace::Namespace(const NamespaceList & parentNamespaces) :
	patternMap( new PatternMap() ), dictionaryMap( new DictionaryMap() ), parents( parentNamespaces ) {
}

Namespace::~Namespace() {
}

uint Namespace::getPatternCount() const {
	return patternMap->map.size();
}

PatternRef Namespace::getPatternByIndex( uint index ) const {
	return patternMap->map.get<1>().at( index );
}

PatternRef Namespace::getPatternByName( const std::string & name ) const {
	PatternMap::Map::iterator i = patternMap->map.find( name );

	if ( i != patternMap->map.end() )
		return *i;

	BOOST_FOREACH( const NamespaceRef parent, parents )
		if ( PatternRef ref = parent->getPatternByName( name ) )
			return ref;

	return PatternRef();
}

PatternRef Namespace::addPattern( const PatternRef & pattern ) {
	patternMap->map.insert( pattern );

	return pattern;
}

uint Namespace::getDictionaryCount() const {
	return dictionaryMap->map.size();
}

DictionaryRef Namespace::getDictionaryByIndex( uint index ) const {
	return dictionaryMap->map.get<1>().at( index );
}

DictionaryRef Namespace::getDictionaryByName( const std::string & name ) const {
	DictionaryMap::Map::iterator i = dictionaryMap->map.find( name );

	if ( i != dictionaryMap->map.end() )
		return *i;

	BOOST_FOREACH( const NamespaceRef parent, parents )
		if ( DictionaryRef ref = parent->getDictionaryByName( name ) )
			return ref;

	return DictionaryRef();
}

DictionaryRef Namespace::addDictionary( const DictionaryRef & dict ) {
	dictionaryMap->map.insert( dict );

	return dict;
}

} // namespace lspl
