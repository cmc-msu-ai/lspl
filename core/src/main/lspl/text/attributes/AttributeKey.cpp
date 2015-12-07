#include "../../base/BaseInternal.h"

#include "AttributeKey.h"
#include "IndexedEntryManager.h"

namespace lspl { namespace text { namespace attributes {

class IndexedAttributeKeyManager : public IndexedEntryManager {
public:
	IndexedAttributeKeyManager() {
		add( 0, "un", "undefined", "неопределено" );
		add( 1, "c", "case", "падеж" );
		add( 2, "n", "number", "число" );
		add( 3, "g", "gender", "род" );
		add( 4, "doc", "degree of comparsion", "степень сравнения" );
		add( 5, "t", "tense", "время" );
		add( 6, "a", "animate", "одушевленность" );
		add( 7, "f", "form", "форма" );
		add( 8, "m", "mode", "наклонение" );
		add( 9, "p", "person", "лицо" );
		add( 10, "r", "reflexive", "неопределено" );
		add( 11, "b", "base", "начальная форма" );
		add( 12, "x", "text", "текст" );
		add( 13, "st", "stem", "основа" );
	};
};

static IndexedAttributeKeyManager indexedAttributeKeyManager;

const AttributeKey AttributeKey::UNDEFINED = AttributeKey( 0 );

const AttributeKey AttributeKey::CASE = AttributeKey( 1 );
const AttributeKey AttributeKey::NUMBER = AttributeKey( 2 );
const AttributeKey AttributeKey::GENDER = AttributeKey( 3 );
const AttributeKey AttributeKey::DOC = AttributeKey( 4 );
const AttributeKey AttributeKey::TENSE = AttributeKey( 5 );
const AttributeKey AttributeKey::ANIMATE = AttributeKey( 6 );
const AttributeKey AttributeKey::FORM = AttributeKey( 7 );
const AttributeKey AttributeKey::MODE = AttributeKey( 8 );
const AttributeKey AttributeKey::PERSON = AttributeKey( 9 );
const AttributeKey AttributeKey::REFLEXIVE = AttributeKey( 10 );
const AttributeKey AttributeKey::BASE = AttributeKey( 11 );
const AttributeKey AttributeKey::TEXT = AttributeKey( 12 );
const AttributeKey AttributeKey::STEM = AttributeKey( 13 );

/**
 * Получить общее количество зарегистрированных аттрибутов
 */
uint AttributeKey::count() {
	return indexedAttributeKeyManager.map.size();
}

/**
 * Найти аттрибут по его аббревиатуре
 */
AttributeKey AttributeKey::findByAbbrevation( const std::string & abbrevation ) {
	auto i = indexedAttributeKeyManager.map.get<0>().find( abbrevation );

	if ( i == indexedAttributeKeyManager.map.get<0>().end() )
		return AttributeKey( 0 );

	return AttributeKey( (*i)->id );
}

/**
 * Найти аттрибут по его имени
 */
AttributeKey AttributeKey::findByName( const std::string & name ) {
	auto i = indexedAttributeKeyManager.map.get<1>().find( name );

	if ( i == indexedAttributeKeyManager.map.get<1>().end() )
		return AttributeKey( 0 );

	return AttributeKey( (*i)->id );
}

/**
 * Зарегистрировать новый аттрибут
 */
AttributeKey AttributeKey::create( const std::string & abbrevation, const std::string & name, const std::string & title ) {
	return AttributeKey( indexedAttributeKeyManager.add( indexedAttributeKeyManager.map.size(), abbrevation, name, title ) );
}

const std::string & AttributeKey::getAbbrevation() const {
	return indexedAttributeKeyManager.at( index ).abbrevation;
}

const std::string & AttributeKey::getName() const {
	return indexedAttributeKeyManager.at( index ).name;
}

const std::string & AttributeKey::getTitle() const {
	return indexedAttributeKeyManager.at( index ).title;
}


} } } // namespace lspl::text::attributes
