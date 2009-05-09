#ifndef _LSPL_TRANSITIONS_PATTERNTRANSITION_H_
#define _LSPL_TRANSITIONS_PATTERNTRANSITION_H_

#include "../base/Range.h"

#include "Transition.h"

#include "../patterns/matchers/Forward.h"
#include "../patterns/Forward.h"

#include <string>
#include <map>

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
 * Сопоставление шаблона в тексте.
 */
class LSPL_EXPORT Match : public Transition {
public:
	typedef std::map<attributes::AttributeKey,attributes::AttributeValue> AttributesMap;
public:
	Match( const text::Node & start, const text::Node & end, const patterns::Pattern & pattern, const patterns::Alternative & alternative, const patterns::matchers::Context & context );
	Match( const text::Node & start, const text::Node & end, const patterns::Pattern & pattern, const AttributesMap & attributes );

	virtual ~Match();

	virtual attributes::AttributeValue getAttribute( attributes::AttributeKey attributeType ) const;

	virtual void dump( std::ostream & out, std::string tabs = "" ) const;

	bool equals( const Match & match ) const;

	bool equals( const Transition & transition ) const {
		if ( transition.type != MATCH )
			return false;

		return equals( *static_cast<const Match*>( &transition ) );
	}

	/**
	 * Получить сопоставленный шаблон
	 */
	const patterns::Pattern & getPattern() const {
		return pattern;
	}

	/**
	 * Получить фрагмент сопоставления по заданному индексу
	 */
	const Fragment & getFragment( uint num ) const;
	uint getFragmentCount() const { return 1; }
private:
	const patterns::Pattern & pattern;

	AttributesMap attributes; // Аттрибуты сопоставления

	mutable Fragment * fragments; // Фрагменты сопоставления, инициализируючтся лениво, при первом запросе
};

} }

#endif /*_LSPL_TRANSITIONS_PATTERNTRANSITION_H_*/
