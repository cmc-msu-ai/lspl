#include "CharacterSets.h"
#include <cctype>

namespace lspl { namespace patterns { namespace parsers {

	uint getCharacterSets(unsigned char c) {
		uint result = 0;
		if (c >= (unsigned char)'0' && c <= (unsigned char)'9')
			result |= LSPL_CHARACTERSETS_DIGIT;
		if (c >= (unsigned char)'a' && c <= (unsigned char)'z')
			result |= LSPL_CHARACTERSETS_LOWERCASE | LSPL_CHARACTERSETS_LATIN;
		if (c >= (unsigned char)'A' && c <= (unsigned char)'Z')
			result |= LSPL_CHARACTERSETS_UPPERCASE | LSPL_CHARACTERSETS_LATIN;
		if ((c >= (unsigned char)'À' && c <= (unsigned char)'ß') || c == (unsigned char)'¨')
			result |= LSPL_CHARACTERSETS_UPPERCASE | LSPL_CHARACTERSETS_CYRILLIC;
		if ((c >= (unsigned char)'à' && c <= (unsigned char)'ÿ') || c == (unsigned char)'¸')
			result |= LSPL_CHARACTERSETS_LOWERCASE | LSPL_CHARACTERSETS_CYRILLIC;
		if (c == (unsigned char)' ' || c == (unsigned char)'\t' || c == (unsigned char)'\n' || c == (unsigned char)'\r')
			result |= LSPL_CHARACTERSETS_SPACE;
		if (ispunct(c & 0x7f))
			result |= LSPL_CHARACTERSETS_PUNCT;
		return result;
	}

	bool isLatin(unsigned char c) {
		return getCharacterSets(c) & LSPL_CHARACTERSETS_LATIN;
	}

	bool isCyrillic(unsigned char c) {
		return getCharacterSets(c) & LSPL_CHARACTERSETS_CYRILLIC;
	}

	bool isSpace(unsigned char c) {
		return getCharacterSets(c) & LSPL_CHARACTERSETS_SPACE;
	}

	bool isPunct(unsigned char c) {
		return getCharacterSets(c) & LSPL_CHARACTERSETS_PUNCT;
	}

	bool isDigit(unsigned char c) {
		return getCharacterSets(c) & LSPL_CHARACTERSETS_DIGIT;
	}

	bool isLowercase(unsigned char c) {
		return getCharacterSets(c) & LSPL_CHARACTERSETS_LOWERCASE;
	}

	bool isUppercase(unsigned char c) {
		return getCharacterSets(c) & LSPL_CHARACTERSETS_UPPERCASE;
	}

	bool isInvalidChar(unsigned char c) {
		return !getCharacterSets(c);
	}
}}}
