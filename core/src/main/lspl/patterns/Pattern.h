#ifndef _LSPL_PATTERNS_PATTERN_H_
#define _LSPL_PATTERNS_PATTERN_H_

#include <vector>

#include "../base/Base.h"
#include "../base/RefCountObject.h"
#include "../base/RefCountPtr.h"

#include "Alternative.h"

#include "../text/Forward.h"

#include <boost/ptr_container/ptr_vector.hpp>

namespace lspl { namespace patterns {

/**
 * Шаблон
 */
class LSPL_EXPORT Pattern : public base::RefCountObject<Pattern> {
public:
	explicit Pattern( const std::string & name ) : name( name ) {}

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
	void addAlternative( Alternative * alt ) {
		alt->pattern = this;
		alternatives.push_back( alt );
	}

	/**
	 * Добавить альтернативы к шаблону
	 */
	template <class PtrContainer>
	void addAlternatives( PtrContainer & r ) {
		alternatives.transfer( alternatives.end(), r.begin(), r.end(), r );
		for( int i = 0; i < alternatives.size(); ++ i ) {
			alternatives[i].pattern = this;
		}
	}

	/**
	 * Обновить список зависимостей альтернативы
	 */
	void updateDependencies();

	/**
	 * Удалить дублирующиеся альтернативы
	 */
	void removeDuplicateAlternatives();

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

public:

	/**
	 * Имя шаблона
	 */
	const std::string name;

	/**
	 * Список альтернатив шаблона
	 */
	boost::ptr_vector<Alternative> alternatives;

private:

	/**
	 * Зависимости шаблона
	 */
	std::vector<const Pattern *> dependencies;
};

} } // namespace lspl::patterns

#endif /*_LSPL_PATTERNS_PATTERN_H_*/
