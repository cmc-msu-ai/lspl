#ifndef _LSPL_PATTERNS_RESTRICTIONS_RESTRICTION_H_
#define _LSPL_PATTERNS_RESTRICTIONS_RESTRICTION_H_

#include "../matchers/Forward.h"

#include "../../base/Base.h"
#include "../../base/Exception.h"
#include "../../text/Forward.h"

#include <ostream>

namespace lspl { namespace patterns { namespace restrictions {

/**
 * Ограничение. Используется для проверки различных условий, заданных в шаблоне, например, условий вхождения в словарь или условий согласования.
 */
class LSPL_EXPORT Restriction {
public:

	Restriction() {
		++ aliveObjectsCount; // Увеличиваем значение счетчика объектов
	}

	virtual ~Restriction() {
		-- aliveObjectsCount; // Уменьшаем значение счетчика объектов
	}

	/**
	 * Проверить, допускает ли ограничение заданную аннотацию в заданном контексте
	 * @param currentAnnotation аннотация
	 * @oaram currentVar переменная, соответствующая аннотации
	 * @param ctx контекст сопоставления
	 * @return true, если допускает, false иначе
	 */
	virtual bool matches( const text::Transition * currentAnnotation, const matchers::Variable currentVar, const matchers::Context & ctx ) const = 0;

	/**
	 * Вывести отладочное представление ограничения в заданный поток
	 * @param out поток, в который выводить представление
	 * @param tabs отступ в начале строки
	 */
	virtual void dump( std::ostream & out, const std::string & tabs = "" ) const = 0;

	/**
	 * Сравнить на равенство с заданным ограничением.
	 *
	 * @param r ограничение, с которым сравнивать
	 * @return true, если ограничения равны
	 */
	virtual bool equals( const Restriction & r ) const = 0;

public:

	bool operator == ( const Restriction & r ) const {
		return equals( r );
	}

	bool operator != ( const Restriction & r ) const {
		return !equals( r );
	}

public:

	/**
	 * Количество объектов в памяти
	 */
	static uint aliveObjectsCount;
};

} } } // namespace lspl::patterns::matchers

#endif//_LSPL_PATTERNS_RESTRICTIONS_RESTRICTION_H_
