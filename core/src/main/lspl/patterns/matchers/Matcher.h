#ifndef _LSPL_MATHCERS_MATCHER_H_
#define _LSPL_MATHCERS_MATCHER_H_

#include "Forward.h"
#include "Context.h"

#include "../restrictions/Restriction.h"

#include "../../text/Forward.h"
#include "../../text/Transition.h"

#include <utility>
#include <vector>
#include <boost/ptr_container/ptr_vector.hpp>

namespace lspl { namespace patterns { namespace matchers {

/**
 * Сопоставитель.
 *
 */
class LSPL_EXPORT Matcher
{
public:
	enum Type {
		WORD, // Слово
		TOKEN, // Лексема
		REGEXP, // Регулярное выражение на лексему
		PATTERN, // Вложенный шаблон
		LOOP // Цикл
	};
public:

	/**
	 * Конструктор.
	 * @param type тип сопоставителя
	 */
	Matcher( Type type );

	/**
	 * Деструктор
	 */
	virtual ~Matcher();

	virtual void dump( std::ostream & out, const std::string & tabs = "" ) const = 0;

	/**
	 * Сравнить на равенство с заданным сопоставителем.
	 *
	 * @param m сопоставитель, с которым сравнивать
	 * @return true, если сопоставители равны
	 */
	virtual bool equals( const Matcher & m ) const;

	/**
	 * Содержит ли сопоставитель переменную
	 */
	virtual bool containsVariable(const Variable &) const;

	/**
	 * Добавить ограничение к сопоставителю
	 * @param r ограничение
	 */
	void addRestriction( restrictions::Restriction * r ) {
		restrictions.push_back( r );
	}

	/**
	 * Добавить к сопоставителю ограничения из вектора
	 * @param r вектор, из которого необходимо добавить ограничения
	 */
	template <class PtrContainer>
	void addRestrictions( PtrContainer & r ) {
		if ( r.begin() != r.end() )
			restrictions.transfer( restrictions.end(), r.begin(), r.end(), r );
	}

public:

	bool operator == ( const Matcher & m ) const {
		return equals( m );
	}

	bool operator != ( const Matcher & m ) const {
		return !equals( m );
	}

protected:
	bool matchRestrictions( const text::Transition & transition, const Context & context ) const;

	void dumpRestrictions( std::ostream & out, const std::string & tabs = "" ) const;
public:

	/**
	 * Тип сопоставителя
	 */
	const Type type;

	/**
	 * Переменная, связаннаяс с сопоставителем
	 */
	Variable variable;

	/**
	 * Набор ограничений на сопоставитель
	 */
	boost::ptr_vector<restrictions::Restriction> restrictions;

	/**
	 * Количество объектов в памяти
	 */
	static uint aliveObjectsCount;
};

typedef std::vector< std::pair<text::TransitionRef,Context> > ChainList;

class LSPL_EXPORT AnnotationMatcher : public Matcher {
public:
	AnnotationMatcher( Type type ) : Matcher( type ) {}

	virtual bool matchTransition( const text::Transition & transition, const Context & context ) const = 0;

	virtual text::TransitionList buildTransitions( const text::Node & node, const Context & context ) const;
};

class LSPL_EXPORT AnnotationChainMatcher : public Matcher {
public:
	AnnotationChainMatcher( Type type ) : Matcher( type ) {}

	/**
	 * Построить группы сопоставлений, начиная с заданного узла
	 */
	virtual void buildChains( const text::Node & node, const Context & context, ChainList & results ) const = 0;

	/**
	 * Построить группыс сопоставлений, начиная с заданного перехода
	 */
	virtual void buildChains( const text::Transition & transition, const Context & context, ChainList & results ) const = 0;

};

inline Matcher* new_clone( const Matcher& r )
{
	return nullptr;
}

} } } // namespace lspl::patterns::matchers

#endif /*_LSPL_MATHCERS_MATCHER_H_*/
