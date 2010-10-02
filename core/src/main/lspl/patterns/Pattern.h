#ifndef _LSPL_PATTERNS_PATTERN_H_
#define _LSPL_PATTERNS_PATTERN_H_

#include "Forward.h"
#include "../text/Forward.h"

#include <boost/ptr_container/ptr_vector.hpp>

namespace lspl { namespace patterns {

/**
 * Шаблон
 */
class LSPL_EXPORT Pattern : public base::RefCountObject, public base::IdentifiedObject<Pattern> {
public:
	explicit Pattern( const std::string & name );
	~Pattern();

	/**
	 * Сбросить в поток отладочное представление шаблона
	 *
	 * @param out поток в который сбрасывать дамп
	 * @param tabs участок, добавляемый в начало каждой строки (используется для структурного представления когда дамп вызывается из дампа вышестоящего объекта)
	 */
	void dump( std::ostream & out, const std::string & tabs = "" ) const;

	/**
	 * Создать новую альтернативу в шаблоне.
	 * Создается новая пустая альтернатива (не содержащая ни одного сопоставителя) и добавляется
	 * в список альтернатив шаблона.
	 *
	 * @param source исходный текст альтернативы
	 *
	 * @return ссылка на новую альтернативу
	 */
	Alternative & newAlternative( const std::string & source );

	/**
	 * Добавить новую альтернативу к шаблону
	 */
	void addAlternative( Alternative * alt );

	/**
	 * Добавить альтернативы к шаблону
	 */
	void addAlternatives( boost::ptr_vector<Alternative> & r );

	/**
	 * Обновить список зависимостей альтернативы
	 */
	void updateDependencies();

	/**
	 * Удалить дублирующиеся альтернативы
	 */
	void removeDuplicateAlternatives();

	/**
	 * Проверить, зависит ли шаблон от заданного шаблона
	 * @param pattern кандидат в зависмости
	 * @param transitive рассматривать ли тразитивные зависимости
	 * @return true, если шаблон действительно зависит от заданного
	 */
	bool dependsOn( const Pattern & pattern, bool transitive = true ) const;

	/**
	 * Получить список зависимостей
	 */
	const std::vector<const Pattern *> & getDependencies() const {
		return dependencies;
	}

	/**
	 * Получить имя шаблона
	 */
	const std::string & getName() const {
		return name;
	}

	/**
	 * Получить исходный текст объявления шаблона, полученный путем склеивания объявлений его альтернатив с переносом строки в качестве разделителя
	 */
	std::string getSource() const;

	/**
	 * Получить список внешних альтернатив шаблона
	 */
	const boost::ptr_vector<Alternative> & getAlternatives() const {
		return alternatives;
	}

private:

	bool deepDependsOn( const Pattern * target, std::vector<const Pattern*> & stack ) const;

public:

	/**
	 * Имя шаблона
	 */
	const std::string name;

	/**
	 * Список альтернатив шаблона
	 */
	boost::ptr_vector<Alternative> alternatives;

    /**
     * Количество объектов в памяти
     */
    static uint aliveObjectsCount;

private:

	/**
	 * Зависимости шаблона
	 */
	std::vector<const Pattern *> dependencies;
};

} } // namespace lspl::patterns

#endif /*_LSPL_PATTERNS_PATTERN_H_*/
