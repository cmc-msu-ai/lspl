#ifndef _LSPL_TEXT_ATTRIBUTES_ATTRIBUTEKEY_H_
#define _LSPL_TEXT_ATTRIBUTES_ATTRIBUTEKEY_H_

#include "../../base/Base.h"

#include <string>
#include <ostream>

namespace lspl { namespace text { namespace attributes {

class LSPL_EXPORT AttributeKey {
public:
	static const AttributeKey UNDEFINED;

	static const AttributeKey CASE;
	static const AttributeKey NUMBER;
	static const AttributeKey GENDER;
	static const AttributeKey DOC;
	static const AttributeKey TENSE;
	static const AttributeKey ANIMATE;
	static const AttributeKey FORM;
	static const AttributeKey MODE;
	static const AttributeKey PERSON;
	static const AttributeKey REFLEXIVE;
	static const AttributeKey STEM;
	static const AttributeKey BASE;
	static const AttributeKey TEXT;
public:

	/**
	 * Получить общее количество зарегистрированных аттрибутов
	 */
	static uint count();

	/**
	 * Найти аттрибут по его аббревиатуре
	 */
	static AttributeKey findByAbbrevation( const std::string & abbrevation );

	/**
	 * Зарегистрировать новый аттрибут
	 */
	static AttributeKey create( const std::string & abbrevation, const std::string & name, const std::string & title );

public:
	explicit AttributeKey( int index ) : index( index ) {}
	explicit AttributeKey() : index( 0 ) {}

	const std::string & getAbbrevation() const;
	const std::string & getName() const;
	const std::string & getTitle() const;

	bool operator == ( const AttributeKey & p ) const {
		return index == p.index;
	}

	bool operator != ( const AttributeKey & p ) const {
		return index != p.index;
	}

	bool operator < ( const AttributeKey & p ) const {
		return index < p.index;
	}
public:
	int index;
};

inline std::ostream & operator << ( std::ostream & out, const AttributeKey & type ) {
	out << type.getAbbrevation();

	return out;
}

} } } // namespace lspl::attributes

#endif /*_LSPL_TEXT_ATTRIBUTES_ATTRIBUTEKEY_H_*/
