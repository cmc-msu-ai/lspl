#ifndef _LSPL_TEXT_WORD_H_
#define _LSPL_TEXT_WORD_H_

#include "Token.h"

#include "../attributes/SpeechPart.h"
#include "../attributes/AttributeContainer.h"

#include <string>

class CFormInfo;

namespace lspl { namespace text { namespace markup {

class LSPL_EXPORT Word : public Transition {
public:
	Word( const text::Node & start, const text::Node & end, const TokenRef & token, const std::string & base, const std::string & stem, attributes::SpeechPart speechPart, uint64 attributes );
	virtual ~Word();

	virtual void dump(std::ostream & out, std::string tabs = "") const;

	/**
	 * Получить значение аттрибута слова
	 */
	virtual attributes::AttributeValue getAttribute( attributes::AttributeKey type ) const;

	std::string getAttributesString() const;

	attributes::AttributeValue getCase() const { return getAttribute(attributes::AttributeKey::CASE); }
	attributes::AttributeValue getNumber() const { return getAttribute(attributes::AttributeKey::NUMBER); }
	attributes::AttributeValue getGender() const { return getAttribute(attributes::AttributeKey::GENDER); }
	attributes::AttributeValue getDoc() const { return getAttribute(attributes::AttributeKey::DOC); }
	attributes::AttributeValue getTense() const { return getAttribute(attributes::AttributeKey::TENSE); }
	attributes::AttributeValue getAnimate() const { return getAttribute(attributes::AttributeKey::ANIMATE); }
	attributes::AttributeValue getForm() const { return getAttribute(attributes::AttributeKey::FORM); }
	attributes::AttributeValue getMode() const { return getAttribute(attributes::AttributeKey::MODE); }
	attributes::AttributeValue getPerson() const { return getAttribute(attributes::AttributeKey::PERSON); }
	attributes::AttributeValue getReflexive() const { return getAttribute(attributes::AttributeKey::REFLEXIVE); }

	const std::string & getToken() const { return token->getToken(); }
	const std::string & getBase() const { return base; }

	attributes::SpeechPart getSpeechPart() const { return speechPart; }

	uint64 getAttributes() const { return attributes; }

	const TokenRef & getTokenRef() const { return token; }

private:

	/**
	 * Часть речи слова
	 */
	const attributes::SpeechPart speechPart;

	const uint64 attributes;

	/**
	 * Аннотация, представляющая лексему слова
	 */
	const TokenRef token;

	/**
	 * Базовая форма слова
	 */
	const std::string base;

	/**
	 * Основа слова
	 */
	const std::string stem;
};

} } } // namespace lspl::text::markup

#endif /*_LSPL_TEXT_WORD_H_*/
