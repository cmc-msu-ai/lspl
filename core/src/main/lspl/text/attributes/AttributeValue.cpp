#include "../../base/BaseInternal.h"
#include "../../base/Exception.h"

#include "AttributeKey.h"
#include "AttributeValue.h"
#include "AttributeContainer.h"
#include "IndexedEntryManager.h"

namespace lspl { namespace text { namespace attributes {

const AttributeValue AttributeValue::UNDEFINED( 0 );

const AttributeValue AttributeValue::NOMINATIVE( 1 );
const AttributeValue AttributeValue::GENITIVE( 2 );
const AttributeValue AttributeValue::DATIVE( 3 );
const AttributeValue AttributeValue::ACCUSATIVE( 4 );
const AttributeValue AttributeValue::INSTRUMENTAL( 5 );
const AttributeValue AttributeValue::PREPOSITIONAL( 6 );
const AttributeValue AttributeValue::UNINFLECTED( 7 );

const AttributeValue AttributeValue::SINGULAR = AttributeValue( 8 );
const AttributeValue AttributeValue::PLURAL = AttributeValue( 9 );

const AttributeValue AttributeValue::MASCULINE = AttributeValue( 10 );
const AttributeValue AttributeValue::FEMININE = AttributeValue( 11 );
const AttributeValue AttributeValue::NEUTER = AttributeValue( 12 );

const AttributeValue AttributeValue::COMPARATIVE = AttributeValue( 13 );
const AttributeValue AttributeValue::SUPERLATIVE = AttributeValue( 14 );
const AttributeValue AttributeValue::NODOC = AttributeValue( 15 );

const AttributeValue AttributeValue::PRESENT = AttributeValue( 16 );
const AttributeValue AttributeValue::PAST = AttributeValue( 17 );
const AttributeValue AttributeValue::FUTURE = AttributeValue( 18 );

const AttributeValue AttributeValue::ANIMATE = AttributeValue( 19 );
const AttributeValue AttributeValue::INANIMATE = AttributeValue( 20 );

const AttributeValue AttributeValue::FULL = AttributeValue( 21 );
const AttributeValue AttributeValue::SHORT = AttributeValue( 22 );

const AttributeValue AttributeValue::INDICATIVE = AttributeValue( 23 );
const AttributeValue AttributeValue::IMPERATIVE = AttributeValue( 24 );
const AttributeValue AttributeValue::CONJUNCTIVE = AttributeValue( 25 );
const AttributeValue AttributeValue::CONDITIONAL = AttributeValue( 26 );

const AttributeValue AttributeValue::FIRST = AttributeValue( 27 );
const AttributeValue AttributeValue::SECOND = AttributeValue( 28 );
const AttributeValue AttributeValue::THIRD = AttributeValue( 29 );

const AttributeValue AttributeValue::REFLEXIVE = AttributeValue( 30 );
const AttributeValue AttributeValue::NOTREFLEXIVE = AttributeValue( 31 );

template<class Class,typename Type,Type Class::*PtrToMember>
inline int fieldOffset() {
	return reinterpret_cast<int>( &( ((Class*)0)->*PtrToMember ) );
}

template<class Class,typename Type,Type Class::*PtrToMember>
inline Class * objectFromField( Type * field ) {
	return reinterpret_cast<Class*>( (char*)( field ) - fieldOffset<Class,Type,PtrToMember>() );
}

template<class Class,typename Type,Type Class::*PtrToMember>
inline Class & objectFromField( Type & field ) {
	return *reinterpret_cast<Class*>( (char*)( &field ) - fieldOffset<Class,Type,PtrToMember>() );
}

class StringManager {
public:
	class Entry {
	public:
		Entry( const std::string & value ) :
			refCount( 0 ), value( value ) {
		}

		uint refCount;
		const std::string value;
	};

	typedef boost::multi_index::multi_index_container<
		boost::shared_ptr< Entry >,
		boost::multi_index::indexed_by<
			boost::multi_index::hashed_unique<
				boost::multi_index::member< Entry, const std::string, &Entry::value >
			>,
			boost::multi_index::random_access<
			>
		>
	> EntryMap;
public:

	const std::string & intern( const std::string & value ) {
		EntryMap::iterator i = map.find( value );

		if ( i == map.end() ) {
			boost::shared_ptr<Entry> entry( new Entry( value ) );
			map.insert( entry );
			return entry->value;
		}

		return (*i)->value;
	}

	void addRef( const std::string & value ) {
		/*Entry * entry = const_cast<Entry*>( objectFromField<Entry, const std::string, &Entry::value>( &value ) );

		entry->refCount ++;*/
	}

	void release( const std::string & value ) {
		/*Entry * entry = const_cast<Entry*>( objectFromField<Entry, const std::string, &Entry::value>( &value ) );

		entry->refCount ++;

		if ( entry.refCount)*/
	}

protected:
	EntryMap map;
};

class IndexedAttributeValueManager : public IndexedEntryManager {
public:
	IndexedAttributeValueManager() {
		add( 0, "un", "undefined", "неопределено" );

		add( 1, "nom", "nominative", "именительный" );
		add( 2, "gen", "genitive", "родительный" );
		add( 3, "dat", "dative", "дательный" );
		add( 4, "acc", "accusative", "винительный" );
		add( 5, "ins", "instrumental", "творительный" );
		add( 6, "prep", "prepositional", "предложный" );
		add( 7, "uninf", "uninflected", "несклоняемое" );

		add( 8, "sing", "singular", "единственное" );
		add( 9, "plur", "plural", "множественное" );

		add( 10, "masc", "masculine", "мужской" );
		add( 11, "fem", "feminine", "женский" );
		add( 12, "neut", "neuter", "средний" );

		add( 13, "com", "comparative", "сравнительная" );
		add( 14, "sup", "superlative", "превосходная" );
		add( 15, "no", "no degree of comparsion", "без степени сравнения" );

		add( 16, "pres", "present", "настоящее" );
		add( 17, "past", "past", "прошедшее" );
		add( 18, "fut", "future", "будущее" );

		add( 19, "anim", "animate", "одушевленный" );
		add( 20, "inan", "inanimate", "неодушелвенный" );

		add( 21, "full", "full", "полная" );
		add( 22, "short", "short", "сокращенная" );

		add( 23, "ind", "indicative", "indicative" );
		add( 24, "imp", "imperative", "императивное" );
		add( 25, "conj", "conjunctive", "conjunctive" );
		add( 26, "cond", "conditional", "условное" );

		add( 27, "1", "first", "первое" );
		add( 28, "2", "second", "второе" );
		add( 29, "3", "third", "третье" );

		add( 30, "yes", "reflexive", "reflexive" );
		add( 31, "no", "not reflexive", "not reflexive" );
	}
};

static StringManager stringAttributeValueManager;
static IndexedAttributeValueManager indexedAttributeValueManager;

/**
 * пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ
 */
uint AttributeValue::indexedCount() {
	return indexedAttributeValueManager.map.size();
}

/**
 * пїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅ пїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ
 */
AttributeValue AttributeValue::findIndexedByAbbrevation( const std::string & abbrevation ) {
	IndexedAttributeValueManager::Iterator i = indexedAttributeValueManager.map.find( abbrevation );

	if ( i == indexedAttributeValueManager.map.end() )
		return AttributeValue( 0 );

	return AttributeValue( (*i)->id );
}

/**
 * пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ
 */
AttributeValue AttributeValue::createIndexed( const std::string & abbrevation, const std::string & name, const std::string & title ) {
	return AttributeValue( indexedAttributeValueManager.add( indexedAttributeValueManager.map.size(), abbrevation, name, title ) );
}

AttributeValue::AttributeValue(const std::string & str)
	: type( AttributeType( AttributeType::STRING_ID ) ), value( reinterpret_cast<long>( &stringAttributeValueManager.intern( str ) ) ) {

	stringAttributeValueManager.addRef( getString() ); // пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅ
}

AttributeValue::AttributeValue( const AttributeValue & att )
	: type( att.type ), value( att.value ) {

	if ( type == AttributeType::STRING )
		stringAttributeValueManager.addRef( getString() ); // пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅ
}

AttributeValue::~AttributeValue() {
	if ( type == AttributeType::STRING )
		stringAttributeValueManager.release( getString() ); // пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅ
}

const std::string & AttributeValue::getAbbrevation() const {
	if ( type != AttributeType::INDEXED )
		return indexedAttributeValueManager.at( 0 ).abbrevation;

	return indexedAttributeValueManager.at( value ).abbrevation;
}

const std::string & AttributeValue::getName() const {
	if ( type != AttributeType::INDEXED )
		return indexedAttributeValueManager.at( 0 ).name;

	return indexedAttributeValueManager.at( value ).name;
}

const std::string & AttributeValue::getTitle() const {
	if ( type != AttributeType::INDEXED )
		return indexedAttributeValueManager.at( 0 ).title;

	return indexedAttributeValueManager.at( value ).title;
}

const std::string * AttributeValue::getStringPtr() const {
	switch ( type.id ) {
	case AttributeType::INDEXED_ID:
		return &getName();
	case AttributeType::STRING_ID:
		return reinterpret_cast<const std::string*>( value );
	case AttributeType::COMPOUND_ID:
		return getContainer().getAttribute( AttributeKey::BASE ).getStringPtr();
	default:
		throw base::Exception( "Unknown value type" );
	}
}

void AttributeValue::dump( std::ostream & out, const std::string & tabs ) const {
	switch ( type.id ) {
	case AttributeType::INDEXED_ID:
		out << getAbbrevation();
		break;
	case AttributeType::STRING_ID:
		out << "'" << getString() << "'";
		break;
	case AttributeType::COMPOUND_ID:
		out << "{ ";
		bool first = true;
		for ( uint i = 0; i < AttributeKey::count(); ++ i ) {
			AttributeValue v = getContainer().getAttribute( AttributeKey( i ) );

			if ( v == AttributeValue::UNDEFINED )
				continue;

			if ( !first )
				out << ", ";
			else
				first = false;

			out << AttributeKey( i ).getAbbrevation() << " / ";

			v.dump( out, tabs + "\t" );
		}
		out << "}";
		break;
	}
}

} } } // namespace lspl::text::attributes
