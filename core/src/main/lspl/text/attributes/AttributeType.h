#ifndef _LSPL_TEXT_ATTRIBUTES_ATTRIBUTETYPE_H_
#define _LSPL_TEXT_ATTRIBUTES_ATTRIBUTETYPE_H_

#include "../../base/Base.h"

namespace lspl { namespace text { namespace attributes {

/**
 * Класс, представляющий тип аттрибута. В настоящий момент существуют три типа аттрибутов: индексированный, строка и составной.
 */
class LSPL_EXPORT AttributeType {
public:
	static const int INDEXED_ID = 1;
	static const int STRING_ID = 2;
	static const int COMPOUND_ID = 3;
	
	/**
	 * Индексированный аттрибут. Такими аттрибутами являются грамматические признаки, а также, неопределенное значение AttributeValue::UNDEFINED
	 */
	static const AttributeType INDEXED;
	
	/**
	 * Строковый аттрибут. Частным случаем строкового аттрибута является основа слова
	 */
	static const AttributeType STRING;
	
	/**
	 * Составной аттрибут
	 */
	static const AttributeType COMPOUND;
public:
	explicit AttributeType( int id ) : id( id ) {}
	explicit AttributeType() : id( 0 ) {}
	
	bool operator == ( const AttributeType & p ) const {
		return id == p.id;
	}
		
	bool operator != ( const AttributeType & p ) const {
		return id != p.id;
	}
	
	bool operator < ( const AttributeType & p ) const {
		return id < p.id;
	}
public:
	int id;
};

} } } // namespace lspl::text::attributes

#endif /*_LSPL_TEXT_ATTRIBUTES_ATTRIBUTETYPE_H_*/
