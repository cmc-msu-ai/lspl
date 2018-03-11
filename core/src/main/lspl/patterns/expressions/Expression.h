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
#include "../matchers/Variable.h"

#include <ostream>
#include <vector>
#include <memory>

namespace lspl { namespace patterns { namespace matchers {

class Context;

} } }

namespace lspl { namespace patterns { namespace expressions {

/**
 * Выражение, вычисляемое в рамках шаблона. Используется для получения значений составных аттрибутов.
 */
class LSPL_EXPORT Expression {
public:
	typedef std::vector<text::attributes::AttributeValue> ValueList;
	typedef std::auto_ptr<ValueList> ValueListPtr;
public:
	Expression();
	virtual ~Expression();

	/**
	 * Вычислить значение выражения
	 * @param currentAnnotation аннотация на которой вычисляется выражение
	 * @param currentVar переменная, соответствующая аннотации
	 * @param ctx контекст
	 * @return значения выражения
	 */
	virtual ValueListPtr evaluate( const text::Transition * currentAnnotation, const matchers::Variable currentVar, const matchers::Context & ctx ) const;

	virtual void evaluateTo( const text::Transition * currentAnnotation, const matchers::Variable currentVar, const matchers::Context & ctx, ValueList & results ) const = 0;

	virtual void dump( std::ostream & out, const std::string & tabs = "" ) const = 0;

	/**
	 * Сравнить на равенство с заданным выражением.
	 *
	 * @param е выражение, с которым сравнивать
	 * @return true, если выражения равны
	 */
	virtual bool equals( const Expression & e ) const = 0;

	/**
	 * Проверить, содержит ли выражение заданную переменную.
	 *
	 * @param var переменная
	 * @return true, если выражение содержит заданную переменную
	 */
	virtual bool containsVariable( matchers::Variable var ) const = 0;

	/**
	 * Проверить, содержит ли выражение хотя бы одну переменную.
	 *
	 * @return true, если выражение содержит хотя бы одну переменную
	 */
	virtual bool containsVariables() const = 0;

	/**
	 * Проверить, содержит ли выражение текущую аннотацию.
	 *
	 * @return true, если выражение содержит текущую аннотацию
	 */
	virtual bool containsCurrentAnnotation() const = 0;

public:

	bool operator == ( const Expression & e ) const {
		return equals( e );
	}

	bool operator != ( const Expression & e ) const {
		return !equals( e );
	}

};

inline std::ostream & operator << ( std::ostream & out, const Expression & val ) {
	val.dump( out );
	return out;
}

inline Expression* new_clone( const Expression& r )
{
	return nullptr;
}

} } }

#endif /* _LSPL_PATTERNS_EXPRESSIONS_EXPRESSION_H_ */
