#ifndef _LSPL_TEXT_TEXTCONFIG_H_
#define _LSPL_TEXT_TEXTCONFIG_H_

#include "../base/Base.h"
#include "Forward.h"

namespace lspl { namespace text {

/**
 * Конфигурация, используемая при построении текста.
 */
class LSPL_EXPORT TextConfig {
public:
	bool analyzePunctuation; // Анализировать ли пунктуацию
	bool analyzeSpaces; // Анализировать ли пробельные символы

	bool splitToSentences; // Разделять ли текст на предложения

	TextConfig() {
		analyzePunctuation = true;
		analyzeSpaces = false;
		splitToSentences = false;
	}
};

} }

#endif /*_LSPL_TEXT_TEXTCONFIG_H_*/
