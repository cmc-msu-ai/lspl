/*
 * Expression.h
 *
 *  Created on: Sep 28, 2008
 *      Author: alno
 */

#ifndef _LSPL_PATTERNS_EXPRESSIONS_EXPRESSION_H_
#define _LSPL_PATTERNS_EXPRESSIONS_EXPRESSION_H_

#include "../../base/Base.h"
#include "../../text/attributes/AttributeValue.h"
#include "../../text/Forward.h"

#include <ostream>

namespace lspl { namespace patterns { namespace matchers {

class Context;

} } }

namespace lspl { namespace patterns { namespace expressions {

/**
 * Выражение, вычисляемое в рамках шаблона. Используется для получения значений составных аттрибутов.
 */
class LSPL_EXPORT Expression {
public:
	Expression();
	virtual ~Expression();

	/**
	 * Вычислить значение выражения
	 * @param annotation аннотация на которой вычисляется выражение
	 * @param ctx контекст
	 * @return значение выражения
	 */
	virtual text::attributes::AttributeValue evaluate( const text::Transition & annotation, const matchers::Context & ctx ) const = 0;

	virtual void dump( std::ostream & out, const std::string & tabs = "" ) const = 0;

	/**
	 * Сравнить на равенство с заданным выражением.
	 *
	 * @param е выражение, с которым сравнивать
	 * @return true, если выражения равны
	 */
	virtual bool equals( const Expression & e ) const = 0;
};

inline std::ostream & operator << ( std::ostream & out, const Expression & val ) {
	val.dump( out );
	return out;
}

} } }

#endif /* _LSPL_PATTERNS_EXPRESSIONS_EXPRESSION_H_ */
