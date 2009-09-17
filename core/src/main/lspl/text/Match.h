#ifndef _LSPL_TRANSITIONS_PATTERNTRANSITION_H_
#define _LSPL_TRANSITIONS_PATTERNTRANSITION_H_

#include "../base/Range.h"

#include "Transition.h"

#include "../patterns/matchers/Forward.h"
#include "../patterns/Forward.h"

#include <string>
#include <map>

#include <boost/ptr_container/ptr_vector.hpp>

namespace lspl { namespace text {

/**
 * Фрагмент, сопоставляемый шаблону или выделяемый им.
 */
class LSPL_EXPORT Fragment : public base::Range {
public:

	/**
	 * Получить текст фрагмента
	 */
	std::string getText() const;

	/**
	 * Получить нормализованный текст фрагмента
	 */
	std::string getNormalizedText() const;

	/**
	 * Получить шаблонизированный текст фрагмента
	 */
	std::string getPatternedText(uint opts) const;

	/**
	 * Получить объект сопоставления
	 */
	const Match & getMatch() const {
		return *match;
	}

public:

	const Match * match; // Сопоставление, которому принадлежит фрагмент

};

/**
 * Вариант сопоставления - хранит конкретный набор сопоставленных элементов
 */
class LSPL_EXPORT MatchVariant : public TransitionList {
public:

	MatchVariant( const patterns::Alternative & alternative );
	MatchVariant( const MatchVariant & variant );

public:
	const patterns::Alternative & alternative;
};

/**
 * Результат сопоставления шаблона в тексте. Состоит из нескольких вариантов сопоставления.
 */
class LSPL_EXPORT Match : public Transition {
public:
	typedef std::map<attributes::AttributeKey,attributes::AttributeValue> AttributesMap;
public:
	/**
	 * При создании результата также верно замечание к методу addVariant
	 */
	Match( const text::Node & start, const text::Node & end, const patterns::Pattern & pattern, MatchVariant * variant, const AttributesMap & attributes );

	virtual ~Match();

	virtual attributes::AttributeValue getAttribute( attributes::AttributeKey attributeType ) const;

	virtual void dump( std::ostream & out, std::string tabs = "" ) const;

	bool equals( const Match & match ) const {
		return equals( match.pattern, match.start, match.end, match.attributes );
	}

	bool equals( const Transition & transition ) const {
		if ( transition.type != MATCH )
			return false;

		return equals( *static_cast<const Match*>( &transition ) );
	}

	bool equals( const patterns::Pattern & p, const Node & start, const Node & end, const AttributesMap & atts ) const;

	/**
	 * Получить сопоставленный шаблон
	 *
	 * @return шаблон, использовавшийся при сопоставлении
	 */
	const patterns::Pattern & getPattern() const {
		return pattern;
	}

	/**
	 * Получить набор вариантов сопоставления
	 *
	 * @return список вариантов сопоставления
	 */
	const boost::ptr_vector<MatchVariant> & getVariants() const {
		return variants;
	}

	/**
	 * Добавить вариант сопоставления.
	 *
	 * Вариант сопоставления является неразделяемым объектом, при добавлении его в результат контроль над его жизненным циклом переходит к результату
	 * и он уничтожит вариант при своем уничтожении. Поэтому при добавлении варианта в результат необходимо исключить его из любых других контейнеров,
	 * которые осуществляют контроль его жизненного цикла, таких как автоматические указатели (auto_ptr) или контейнеры указателей (ptr_containers).
	 *
	 * @param variant вариант сопоставления
	 */
	void addVariant( MatchVariant * variant ) {
		variants.push_back( variant );
	}

	/**
	 * Получить фрагмент сопоставления по заданному индексу
	 */
	const Fragment & getFragment( uint num ) const;
	uint getFragmentCount() const { return 1; }
private:
	const patterns::Pattern & pattern;

	boost::ptr_vector<MatchVariant> variants; // Различные варианты сопоставления
	AttributesMap attributes; // Аттрибуты сопоставления

	mutable Fragment * fragments; // Фрагменты сопоставления, инициализируючтся лениво, при первом запросе
};

} }

#endif /*_LSPL_TRANSITIONS_PATTERNTRANSITION_H_*/
