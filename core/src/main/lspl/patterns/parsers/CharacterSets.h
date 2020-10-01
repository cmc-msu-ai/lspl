#ifndef _LSPL_UTILS_CHARACTERSETS_H_
#define _LSPL_UTILS_CHARACTERSETS_H_

#include "../../base/BaseInternal.h"


#define LSPL_CHARACTERSETS_LATIN 1
#define LSPL_CHARACTERSETS_CYRILLIC 2
#define LSPL_CHARACTERSETS_SPACE 4
#define LSPL_CHARACTERSETS_PUNCT 8
#define LSPL_CHARACTERSETS_DIGIT 16
#define LSPL_CHARACTERSETS_LOWERCASE 32
#define LSPL_CHARACTERSETS_UPPERCASE 64

namespace lspl { namespace patterns { namespace parsers {
	uint getCharacterSets(unsigned char);
	bool isLatin(unsigned char);
	bool isCyrillic(unsigned char);
	bool isSpace(unsigned char);
	bool isPunct(unsigned char);
	bool isDigit(unsigned char);
	bool isLowercase(unsigned char);
	bool isUppercase(unsigned char);
	bool isInvalidChar(unsigned char);
}}}


#endif /* _LSPL_UTILS_RUSCONSTS_H_ */
