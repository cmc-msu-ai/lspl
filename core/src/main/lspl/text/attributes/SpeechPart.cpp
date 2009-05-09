#include "../../base/BaseInternal.h"

#include "SpeechPart.h"

namespace lspl { namespace text { namespace attributes {

const SpeechPart SpeechPart::WORD = SpeechPart( 0 );
const SpeechPart SpeechPart::NOUN = SpeechPart( 1 );
const SpeechPart SpeechPart::ADJECTIVE = SpeechPart( 2 );
const SpeechPart SpeechPart::VERB = SpeechPart( 3 );
const SpeechPart SpeechPart::PARTICIPLE = SpeechPart( 4 );
const SpeechPart SpeechPart::ADVERBIAL_PARTICIPLE = SpeechPart( 5 );
const SpeechPart SpeechPart::PRONOUN = SpeechPart( 6 );
const SpeechPart SpeechPart::ADVERB = SpeechPart( 7 );
const SpeechPart SpeechPart::CONJUNCTION = SpeechPart( 8 );
const SpeechPart SpeechPart::PREPOSITION = SpeechPart( 9 );
const SpeechPart SpeechPart::PARTICLE = SpeechPart( 10 );
const SpeechPart SpeechPart::INTERJECTION = SpeechPart( 11 );
const SpeechPart SpeechPart::NUMERAL = SpeechPart( 12 );

const std::string SpeechPart::ABBREVATIONS[] = { "W", "N", "A", "V", "Pa", "Ap", "Pn",
			"Av", "Cn", "Pr", "Pt", "Int", "Num" };

const std::string SpeechPart::NAMES[] = { "Any", "Noun", "Adjective", "Verb", "Participle", "Adverbal participle", "Pronoun",
			"Adverb", "Conjunction", "Preposition", "Particle", "Interjection", "Numeral" };

const std::string SpeechPart::TITLES[] = { "Произвольная", "Существительное", "Прилагательное", "Глагол", "Причастие", "Деепричастие", "Местоимение",
			"Наречие", "Союз", "Предлог", "Частица", "Междометие", "Числительное" };

const std::string SpeechPart::PLURALTITLES[] = { "Произвольные", "Существительные", "Прилагательные", "Глаголы", "Причастия", "Деепричастия", "Местоимения",
			"Наречия", "Союзы", "Предлоги", "Частицы", "Междометия", "Числительные" };

} } } // namespace lspl::text::attributes
