#ifndef _LSPL_TEXT_ATTRIBUTES_ATTRIBUTECONTAINER_H_
#define _LSPL_TEXT_ATTRIBUTES_ATTRIBUTECONTAINER_H_

#include "../../base/Base.h"

#include "AttributeKey.h"
#include "AttributeValue.h"

#include <string>

namespace lspl { namespace text { namespace attributes {

/**
 * Класс, представляющий объект, обладающий аттрибутами
 */
class LSPL_EXPORT AttributeContainer
{
public:
	AttributeContainer() {};
	virtual ~AttributeContainer() {};
	
	/**
	 * Получить значение аттрибута по ключу. Если аттрибут с заданным ключом не найден, возвращается значение AttributeValue::UNDEFINED
	 * 
	 * @param key ключ извлекаемого аттрибута
	 */
	virtual AttributeValue getAttribute( AttributeKey key ) const = 0;
};

} } } // namespace lspl::text::attributes

#endif /*_LSPL_TEXT_ATTRIBUTES_ATTRIBUTECONTAINER_H_*/
