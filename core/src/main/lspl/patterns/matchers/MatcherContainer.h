#ifndef _LSPL_MATCHERS_MATCHERCONTAINER_H_
#define _LSPL_MATCHERS_MATCHERCONTAINER_H_

#include "Forward.h"
#include "../../base/Base.h"
#include "../../text/attributes/SpeechPart.h"

#include <boost/ptr_container/ptr_vector.hpp>

#include "../Forward.h"

namespace lspl { namespace patterns { namespace matchers {

/**
 * Контейнер сопоставителей
 */
class LSPL_EXPORT MatcherContainer {
public:
	/**
	 * Конструктор по умолчанию
	 */
	MatcherContainer();

	/**
	 * Деструктор
	 */
	virtual ~MatcherContainer();

	/**
	 * Создать новый сопоставитель лексем
	 */
	TokenMatcher & newTokenMatcher( const std::string & token );

	/**
	 * Создать новый сопоставитель слов
	 */
	WordMatcher & newWordMatcher( text::attributes::SpeechPart speechPart );

	/**
	 * Создать новый сопоставитель шаблона
	 */
	PatternMatcher & newPatternMatcher( const Pattern & pattern );

	/**
	 * Создать новый сопоставитель цикла
	 */
	LoopMatcher & newLoopMatcher( uint min, uint max );

	/**
	 * Добавить сопоставитель в контейнер
	 * @param сопоставитель
	 */
	void addMatcher( Matcher * matcher );

	/**
	 * Добавить набор сопоставителей из промежутка в контейнер
	 * @param from начало промежутка
	 * @param to конец промежутка
	 */
	template <typename StartIterator,typename EndIterator>
	void addMatchers( const StartIterator & from, const EndIterator & to ) {
		for ( StartIterator t = from; t != to; ++ t )
			addMatcher( *t );
	}

	/**
	 * Добавить к сопоставителю ограничения из вектора
	 * @param r вектор, из которого необходимо добавить ограничения
	 */
	template <class PtrContainer>
	void addMatchers( PtrContainer & r ) {
		matchers.transfer( matchers.end(), r.begin(), r.end(), r );
	}

	/**
	 * Добавить набор сопоставителей из промежутка в контейнер
	 * @param b начало промежутка
	 * @param e конец промежутка
	 */
	template <class Iterator,class PtrContainer>
	void addMatchers( Iterator b, Iterator e, PtrContainer & r ) {
		matchers.transfer( matchers.end(), b, e, r );
	}

	/**
	 * Получить количество сопоставителей в контейнере. Этот метод возвращает размер массива сопоставителей, т.е. кол-во как существующих сопоставителей, так и путсых ссылок на них в контейнере.
	 * @return кол-во сопоставителей
	 */
	uint getMatcherCount() const {
		return matchers.size();
	};

	/**
	 * Получить список сопоставителей в контейнере
	 */
	const boost::ptr_vector<Matcher> & getMatchers() const {
		return matchers;
	}

	boost::ptr_vector<Matcher> & getMatchers() {
		return matchers;
	}

	/**
	 * Получить сопоставитель из контейнера по индексу
	 */
	const Matcher & getMatcher( uint i ) const {
		return matchers[ i ];
	}

private:

	/**
	 * Список сопоставителей в контейнере
	 */
	boost::ptr_vector<Matcher> matchers;
};

} } } // namespace lspl::patterns::matchers

#endif /*_LSPL_MATCHERS_MATCHERCONTAINER_H_*/
