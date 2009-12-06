#ifndef _LSPL_TRANSITIONS_PATTERNTRANSITION_H_
#define _LSPL_TRANSITIONS_PATTERNTRANSITION_H_

#include "../base/Exception.h"

#include "Transition.h"

#include "../patterns/matchers/Forward.h"
#include "../patterns/Forward.h"

#include "../transforms/TransformResult.h"

#include <string>
#include <map>

#include <boost/ptr_container/ptr_vector.hpp>

namespace lspl { namespace text {

/**
 * Вариант сопоставления - хранит конкретный набор сопоставленных элементов
 */
class LSPL_EXPORT MatchVariant : public base::RefCountObject<MatchVariant>, public TransitionList {
public:

	MatchVariant( const patterns::Alternative & alternative );
	MatchVariant( const MatchVariant & variant );
	~MatchVariant();

	/**
	 * Получить результат преобразования, определяемого альтернативой
	 */
	template <typename Result>
	Result getTransformResult() const {
		if ( !transformResult )
			transformResult = calculateTransformResult(); // Вычисляем значение преобразования, если его еще нет

		return transformResult->getValue<Result>();
	}

private:

	transforms::TransformResult * calculateTransformResult() const;

public:
	const patterns::Alternative & alternative;
	const Match * match;
	int index;
private:

	/**
	 * Преобразование, осуществляемое альтернативой
	 */
	mutable transforms::TransformResult * transformResult;
};

class LSPL_EXPORT MatchVariantContainer : public Transition {
public:
	typedef std::map<attributes::AttributeKey,attributes::AttributeValue> AttributesMap;
public:
	MatchVariantContainer( Type type, const text::Node & start, const text::Node & end, const patterns::Pattern & pattern, const AttributesMap & attributes );
	virtual ~MatchVariantContainer();

	virtual attributes::AttributeValue getAttribute( attributes::AttributeKey attributeType ) const;

	/**
	 * Получить сопоставленный шаблон
	 *
	 * @return шаблон, использовавшийся при сопоставлении
	 */
	const patterns::Pattern & getPattern() const {
		return pattern;
	}

	const MatchVariantRef & getVariant( uint index ) const {
		return variants[ index ];
	}

	const uint getVariantCount() const {
		return variants.size();
	}

	/**
	 * Получить набор вариантов сопоставления
	 *
	 * @return список вариантов сопоставления
	 */
	const MatchVariantList & getVariants() const {
		return variants;
	}

protected:
	const patterns::Pattern & pattern;

	AttributesMap attributes; // Аттрибуты сопоставления
	MatchVariantList variants; // Различные варианты сопоставления
};

/**
 * Результат сопоставления шаблона с дополнительными ограничениями.
 */
class LSPL_EXPORT RestrictedMatch : public MatchVariantContainer {
public:
	RestrictedMatch( const text::Node & start, const text::Node & end, const patterns::Pattern & pattern, MatchVariant * variant, const AttributesMap & attributes );
	virtual ~RestrictedMatch();

	virtual void dump( std::ostream & out, std::string tabs = "" ) const;

	void addVariant( MatchVariant * variant ) {
		variants.push_back( variant );
	}
};

/**
 * Результат сопоставления шаблона в тексте. Состоит из нескольких вариантов сопоставления.
 */
class LSPL_EXPORT Match : public MatchVariantContainer {

public:
	Match( const text::Node & start, const text::Node & end, const patterns::Pattern & pattern, MatchVariant * variant, const AttributesMap & attributes );
	virtual ~Match();

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
	 * Добавить вариант сопоставления.
	 *
	 * @param variant вариант сопоставления
	 */
	void addVariant( MatchVariant * variant ) {
		variant->index = variants.size();
		variant->match = this;

		variants.push_back( variant );
	}

};

} }

#endif /*_LSPL_TRANSITIONS_PATTERNTRANSITION_H_*/
