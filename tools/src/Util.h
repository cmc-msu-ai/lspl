/*
 * Author: Antonov Vadim (avadim@gmail.com)
 *
 * This class represents common utils.
 */

#ifndef __LSPL_UTIL
#define __LSPL_UTIL

#include <string>

#include "lspl/text/Text.h"
#include "lspl/utils/Conversion.h"

namespace lspl {

class Util {
 public:
	static lspl::utils::Conversion in;
	static lspl::utils::Conversion out;
	// Read text from the file, convert it from the UTF8 to the WIN-1251 encoding.
	static std::string LoadTextFromFile(const char *filename);
	// Load text from the file and convert it into the lspl representation.
	static text::TextRef LoadText(const char *text_filename);
	static std::string Trim(const std::string &text);
	static int CountWords(const std::string &text);
};

} // namespace lspl.

#endif // __LSPL_UTIL

