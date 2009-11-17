/*
 * Author: Antonov Vadim (avadim@gmail.com)
 *
 * This class represents common utils.
 */

#ifndef __LSPL_UTIL
#define __LSPL_UTIL

#include <string>
#include <vector>

#include "lspl/patterns/Pattern.h"
#include "lspl/text/Text.h"
#include "lspl/transforms/Normalization.h"
#include "lspl/utils/Conversion.h"

namespace lspl {

class Util {
 public:
	static const std::string delimiters;
	static lspl::utils::Conversion in;
	static lspl::utils::Conversion out;
	static transforms::Normalization normalization;
 public:
	// Read text from the file, convert it from the UTF8 to the WIN-1251 encoding.
	static std::string LoadTextFromFile(const char *filename);
	// Load text from the file and convert it into the lspl representation.
	static text::TextRef LoadText(const char *text_filename);
	// Remove first and last spaces.
	static std::string Trim(const std::string &text);
	// Count words in srting.
	static int CountWords(const std::string &text);
	// Extract words
	static void ExtractWords(const std::string &text,
			std::vector<std::string> &words);
	// Calculate words by pattern.
	static bool BuildWordsByPattern(const std::string &text,
			const patterns::PatternRef &pattern,
			std::map<std::string, std::string> &words);
	// Convert terms to text representation.
	static void ConvertToText(const std::vector<std::string> &terms,
			std::vector<text::TextRef> &terms_text);
	// Convert to text one string.
	static text::TextRef ConvertToText(const std::string &term);
	// Normalize string. Doesn't work now.
	static std::string Normalize(const std::string &term);
	// Normalize pattern. Remove all conditions and useless spaces.
	static std::string NormalizePattern(const std::string &pattern);
	// Checks, that full text equals to pattern. If true, that retruns
	// normalized text. Else returns "".
	static std::string GetNormalizedMatch(text::TextRef text,
			patterns::PatternRef pattern);
	// Change pattern with set of words.
	static std::string BuildPattern(std::string &pattern,
			std::map<std::string, std::string> &words);
};

} // namespace lspl.

#endif // __LSPL_UTIL

