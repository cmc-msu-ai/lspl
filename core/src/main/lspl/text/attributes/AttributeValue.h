#ifndef _LSPL_TEXT_ATTRIBUTES_ATTRIBUTEVALUE_H_
#define _LSPL_TEXT_ATTRIBUTES_ATTRIBUTEVALUE_H_

#include "../../base/Base.h"
#include "../../base/Exception.h"

#include "AttributeType.h"

#include <string>
#include <ostream>

namespace lspl { namespace text { namespace attributes {

class AttributeContainer;

class LSPL_EXPORT AttributeValue {
public:
	static const AttributeValue UNDEFINED;

	static const AttributeValue NOMINATIVE; // Именительный
	static const AttributeValue GENITIVE; // Родительный
	static const AttributeValue DATIVE; // Дательный
	static const AttributeValue ACCUSATIVE; // Винительный
	static const AttributeValue INSTRUMENTAL; // Творительный
	static const AttributeValue PREPOSITIONAL; // Предложный
	static const AttributeValue UNINFLECTED; // Несклоняемое

	static const AttributeValue SINGULAR; // Единственное
	static const AttributeValue PLURAL; // Множественное

	static const AttributeValue MASCULINE; // Мужской
	static const AttributeValue FEMININE; // Женский
	static const AttributeValue NEUTER; // Без рода

	static const AttributeValue COMPARATIVE; // Сравнительная
	static const AttributeValue SUPERLATIVE; // Превосходная
	static const AttributeValue NODOC; // Без степени сравнения

	static const AttributeValue PRESENT; // Настоящее
	static const AttributeValue PAST; // Прошлое
	static const AttributeValue FUTURE; // Будущее

	static const AttributeValue ANIMATE; // Одушевленное
	static const AttributeValue INANIMATE; // Неодушевленное

	static const AttributeValue FULL; // Полная форма
	static const AttributeValue SHORT; // Скоращенная форма

	static const AttributeValue INDICATIVE;
	static const AttributeValue IMPERATIVE;
	static const AttributeValue CONJUNCTIVE;
	static const AttributeValue CONDITIONAL;

	static const AttributeValue FIRST; // Первое
	static const AttributeValue SECOND; // Второе
	static const AttributeValue THIRD; // Третье

	static const AttributeValue REFLEXIVE; // Возвратный
	static const AttributeValue NOTREFLEXIVE; // Невозвратный
public:

	/**
	 * Получить количество индексированных значений аттрибутов
	 */
	static uint indexedCount();

	/**
	 * Найти индексированное значение аттрибута по его аббревиатуре
	 */
	static AttributeValue findIndexedByAbbrevation( const std::string & abbrevation );

	/**
	 * Зарегистрировать новое индексированное значение аттрибута
	 */
	static AttributeValue createIndexed( const std::string & abbrevation, const std::string & name, const std::string & title );
public:
	AttributeValue() : type( AttributeType( AttributeType::INDEXED_ID ) ), value( 0 ) {}
	AttributeValue( int index ) : type( AttributeType( AttributeType::INDEXED_ID ) ), value( index ) {}
	AttributeValue( const AttributeContainer & container ) : type( AttributeType( AttributeType::COMPOUND_ID ) ), value( reinterpret_cast<long>( &container ) ) {}
	AttributeValue( const std::string & str );

	AttributeValue( const AttributeValue & att );
	~AttributeValue();

	bool operator == ( const AttributeValue & p ) const {
		if ( type != p.type )
			return false;

		if ( type == AttributeType::INDEXED || type == AttributeType::COMPOUND )
			return value == p.value;

		return *reinterpret_cast<const std::string*>( value ) == *reinterpret_cast<const std::string*>( p.value );
	}

	/**
	 * Получить cтроковое значение аттрибута.
	 * В зависимости от типа аттрибута понятие его строкового значения различается.
	 * Для строкового аттрибута это его единственно возможноез значение.
	 * Для индексированного аттрибута это его имя.
	 * Для составного аттрибута это строковое значение его аттрибута TEXT.
	 */
	const std::string & getString() const {
		return *getStringPtr();
	}

	/**
	 * Получить указатель на cтроковое значение аттрибута.
	 * В зависимости от типа аттрибута понятие его строкового значения различается.
	 * Для строкового аттрибута это его единственно возможноез значение.
	 * Для индексированного аттрибута это его имя.
	 * Для составного аттрибута это строковое значение его аттрибута TEXT.
	 */
	const std::string * getStringPtr() const;

	/**
	 * Получить контейнер для составного значения
	 */
	const AttributeContainer & getContainer() const {
		return *reinterpret_cast<const AttributeContainer*>( value );
	}

	/**
	 * Получить указатель на контейнер для составного значения
	 */
	const AttributeContainer * getContainerPtr() const {
		if ( type != AttributeType::COMPOUND )
			return 0;

		return reinterpret_cast<const AttributeContainer*>( value );
	}

	/**
	 * Получить аббревиатуру индексированного значения
	 */
	const std::string & getAbbrevation() const;

	/**
	 * Получить имя индексированного значения
	 */
	const std::string & getName() const;

	/**
	 * Получить заголовок индексированного значения
	 */
	const std::string & getTitle() const;

	void dump( std::ostream & os, const std::string & tabs = "" ) const;

	bool operator != ( const AttributeValue & p ) const {
		return !operator==( p );
	}

	bool operator < ( const AttributeValue & p ) const {
		return type < p.type || value < p.value;
	}
public:
	AttributeType type;
	long value;
};

inline std::ostream & operator << ( std::ostream & out, const AttributeValue & val ) {
	val.dump( out );
	return out;
}

} } } // namespace lspl::text::attributes

#endif /*_LSPL_TEXT_ATTRIBUTES_ATTRIBUTEVALUE_H_*/
