#ifndef _LSPL_TEXT_ATTRIBUTES_SPEECHPART_H_
#define _LSPL_TEXT_ATTRIBUTES_SPEECHPART_H_

#include "../../base/Base.h"

#include <string>

namespace lspl { namespace text { namespace attributes {

/**
 * Часть речи
 */
class LSPL_EXPORT SpeechPart {
public:

	static const SpeechPart WORD; // Произвольная часть речи
	static const SpeechPart NOUN; // Существительное
	static const SpeechPart ADJECTIVE; // Прилагательное
	static const SpeechPart VERB; // Глагол
	static const SpeechPart PARTICIPLE; // Причастие
	static const SpeechPart ADVERBIAL_PARTICIPLE; // Деепричастие
	static const SpeechPart PRONOUN; // Местоимение
	static const SpeechPart ADVERB; // Наречие
	static const SpeechPart CONJUNCTION; // Союз
	static const SpeechPart PREPOSITION; // Предлог
	static const SpeechPart PARTICLE; // Частица
	static const SpeechPart INTERJECTION; // Междометие
	static const SpeechPart NUMERAL; // Числительное
	
	static const uint COUNT = 13;
	static const std::string ABBREVATIONS[COUNT];
	static const std::string NAMES[COUNT];
	static const std::string TITLES[COUNT];
	static const std::string PLURALTITLES[COUNT];	
public:

	SpeechPart() : id( 0 ) {}
	
	explicit SpeechPart(int id) :
		id( id ) {}

	/**
	 * Получить аббревиатуру части речи
	 */
	const std::string & getAbbrevation() const {
		return ABBREVATIONS[ id ];
	}
	
	/**
	 * Получить название части речи
	 */
	const std::string & getName() const {
		return NAMES[ id ];
	}
	
	/**
	 * Получить название части речи
	 */
	const std::string & getTitle() const {
		return TITLES[ id ];
	}
	
	/**
	 * Получить название части речи в множественном числе
	 */
	const std::string & getPluralTitle() const {
		return PLURALTITLES[ id ];
	}
	
	bool operator == ( const SpeechPart & p ) const {
		return id == p.id;
	}
	
	bool operator != ( const SpeechPart & p ) const {
		return id != p.id;
	}

public:

	int id;
};

} } } // namespace lspl::text::attributes

#endif /*_LSPL_TEXT_ATTRIBUTES_SPEECHPART_H_*/
