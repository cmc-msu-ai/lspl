#ifndef _LSPL_NAMESPACE_H_
#define _LSPL_NAMESPACE_H_

#include "base/Base.h"
#include "base/RefCountObject.h"

#include "patterns/Forward.h"
#include "dictionaries/Dictionary.h"

#include <boost/scoped_ptr.hpp>

namespace lspl {

LSPL_REFCOUNT_FORWARD(Namespace);

/**
 * Пространство имен.
 * Хранит ссылки на определенные шаблоны, словари и прочие объекты.
 * Использование нескольких различных пространств имен позволяет ппроизводить обработку одного или
 * нескольких текстов с помощью различных наборов шаблонов, которы, возможно пересекаются по именам.
 */
class LSPL_EXPORT Namespace : public base::RefCountObject, public base::IdentifiedObject<Namespace>
{
public:
	Namespace();
	Namespace(const NamespaceRef & parentNamespace);
	Namespace(const NamespaceList & parentNamespaces);
	virtual ~Namespace();

	/**
	 * Получить количество шаблонов в пространстве имен
	 *
	 * @return количество шаблонов
	 */
	uint getPatternCount() const;

	/**
	 * Получить шаблон по индексу
	 *
	 * @param index индекс шаблона
	 * @return шаблон
	 */
	patterns::PatternRef getPatternByIndex( uint index ) const;

	/**
	 * Получить шаблон по имени
	 *
	 * @param name имя шаблона
	 * @return шаблон
	 */
	patterns::PatternRef getPatternByName( const std::string & name ) const;

	/**
	 * Добавить шаблон в пространство имен
	 *
	 * @param pattern шаблон
	 */
	patterns::PatternRef addPattern( const patterns::PatternRef & pattern );

	/**
	 * Получить количество словарей в пространстве имен
	 *
	 * @return количество словарей
	 */
	uint getDictionaryCount() const;

	/**
	 * Получить словарь по индексу
	 *
	 * @param index индекс словаря
	 * @return словарь
	 */
	dictionaries::DictionaryRef getDictionaryByIndex( uint index ) const;

	/**
	 * Получить словарь по имени
	 *
	 * @param name имя словаря
	 * @return словарь
	 */
	dictionaries::DictionaryRef getDictionaryByName( const std::string & name ) const;

	/**
	 * Добавить словарь в пространство имен
	 *
	 * @param dictionary словарь
	 */
	dictionaries::DictionaryRef addDictionary( const dictionaries::DictionaryRef & dictionary );

	const NamespaceList & getParents() const {
		return parents;
	}

private:
	class PatternMap;
	class DictionaryMap;
private:
	boost::scoped_ptr<PatternMap> patternMap;
	boost::scoped_ptr<DictionaryMap> dictionaryMap;
	NamespaceList parents;
};

}

#endif /*_LSPL_NAMESPACE_H_*/
