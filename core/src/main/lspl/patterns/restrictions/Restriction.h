#ifndef _LSPL_PATTERNS_RESTRICTIONS_RESTRICTIONS_H_
#define _LSPL_PATTERNS_RESTRICTIONS_RESTRICTIONS_H_

#include "../matchers/Forward.h"

#include "../../base/Base.h"
#include "../../base/Exception.h"
#include "../../text/Forward.h"

#include <ostream>

namespace lspl { namespace patterns { namespace restrictions {

/**
 * Ограничение на сопоставитель.
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
	 * @param annotation аннотация
	 * @param ctx контексте
	 * @return true, если допускает
	 */
	virtual bool matches( const text::Transition & annotation, const matchers::Context & ctx ) const = 0;

	virtual void dump( std::ostream & out, const std::string & tabs = "" ) const = 0;

	/**
	 * Сравнить на равенство с заданным ограничением.
	 *
	 * @param r ограничение, с которым сравнивать
	 * @return true, если ограничения равны
	 */
	virtual bool equals( const Restriction & r ) const = 0;

public:

	/**
	 * Количество объектов в памяти
	 */
	static uint aliveObjectsCount;
};

} } } // namespace lspl::patterns::matchers

#endif//_LSPL_PATTERNS_RESTRICTIONS_RESTRICTIONS_H_
