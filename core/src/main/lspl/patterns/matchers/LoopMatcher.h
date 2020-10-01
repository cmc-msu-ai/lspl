#ifndef _LSPL_MATCHERS_LOOPMATCHER_H_
#define _LSPL_MATCHERS_LOOPMATCHER_H_

#include "Forward.h"
#include "Matcher.h"
#include "MatcherContainer.h"

#include <boost/ptr_container/ptr_vector.hpp>

namespace lspl { namespace patterns { namespace matchers {

/**
 * Сопоставитель цикла.
 *
 * Сопоставитель цикла используется для обработки конструкций повторения последовательности (а также, опциональных последовательностей),
 * которые описываются с помощью квадратных и фигурных скобок. Например: [A] N - опцональное прилагательное перед существительным,
 * {A} N - произвольное число прилагательных перед существительным.
 *
 * Сопоставитель имеет два управляющих параметра: минимальное и максимальное число повторений последовательности. У обоих параметров
 * значение 0 обозначает "неограниченно". В зависимости от типа скобок, начальные параметры <0,1> и <0,0> для квадратных и фигурных соответственно.
 * В случае фигурных скобок параметры могут быть заданы явно.
 *
 * Сопоставитель содержит несколько альтернативных последовательностей, которые могут повторяться.
 */
class LSPL_EXPORT LoopMatcher : public AnnotationChainMatcher
{
public:
	/**
	 * Конструктор по умолчанию. Значения параметров minLoops и maxLoops устанавливаются в 0.
	 */
	LoopMatcher();

	/**
	 * Конструктор сопоставителя цикла.
	 * @param min минимальное число повторений. 0 - без ограничений.
	 * @param max максимальное число повторений. 0 - без ограничений.
	 */
	LoopMatcher( uint min, uint max, bool is_permutation = false );


	/**
	 * Содержит ли переменную?
	 */
	virtual bool containsVariable(const Variable &) const;

	/**
	 * Деструктор
	 */
	virtual ~LoopMatcher();

	/**
	 * Добавить альтернативу в сопоставитель
	 * @param alternative альтернатива
	 */
	void addAlternative( MatcherContainer * alternative ) {
		alternatives.push_back( alternative );
	}

	/**
	 * Создать новую альтернативу
	 * @return ссылка на альтерантиву
	 */
	MatcherContainer & newAlternative() {
		MatcherContainer * a = new MatcherContainer();
		alternatives.push_back( a );
		return *a;
	}

	void buildChains( const text::Node & node, const Context & context, ChainList & results ) const;

	void buildChains( const text::Transition & transition, const Context & context, ChainList & results ) const;

	virtual void dump( std::ostream & out, const std::string & tabs = "" ) const;
public:

	/**
	 * Минимальное число повторений. 0 - без ограничений.
	 */
	uint minLoops;

	/**
	 * Максимальное число повторений. 0 - без ограничений.
	 */
	uint maxLoops;

	/**
	 * Является ли перестановкой?
	 */
	bool is_permutation;

	/**
	 * Список альтернатив последовательности.
	 */
	boost::ptr_vector<MatcherContainer> alternatives;
};

} } } // namespace lspl::patterns::matchers

#endif /*_LSPL_MATCHERS_LOOPMATCHER_H_*/
