/*
 * Author: Antonov Vadim (avadim@gmail.com)
 *
 * This class represents common utils.
 */

#ifndef __LSPL_UTIL
#define __LSPL_UTIL

#include <string>
#include <vector>

#include "lspl/Namespace.h"
#include "lspl/patterns/Pattern.h"
#include "lspl/patterns/restrictions/AndRestriction.h"
#include "lspl/text/readers/PlainTextReader.h"
#include "lspl/text/Text.h"
#include "lspl/transforms/Normalization.h"
#include "lspl/utils/Conversion.h"

namespace lspl {

enum LoadSimilarPatternsState {
	LOADSTATE_NEW,
	LOADSTATE_SIMILARS
};

class TTerm;
typedef std::vector<TTerm> TTerms;

class Util {
 public:
	static const std::string delimiters;
	static const std::string spaces;
	static lspl::utils::Conversion in;
	static lspl::utils::Conversion out;
	static transforms::Normalization normalization;
	static text::readers::PlainTextReader reader;
 public:
	// Read text from the file, convert it from the UTF8 to the WIN-1251 encoding.
	static std::string LoadTextFromFile(const char *filename);
	// Load text from the file and convert it into the lspl representation.
	static text::TextRef LoadText(const char *text_filename);
	// Remove first and last spaces.
	static std::string Trim(const std::string &text);
	// Count words in srting.
	static int CountWords(const std::string &text);
	// Extract words.
	static void ExtractWords(const std::string &text,
			std::vector<std::string> &words);
	// Extract words and delimiters.
	static void ExtractWordsAndDelimiters(const std::string &text,
			std::vector<std::string> &words);
	// Calculate words by pattern.
	static bool BuildWordsByPattern(const std::string &text,
			const patterns::PatternRef &pattern,
			std::map<std::string, std::string> &words);
	// Convert terms to text representation.
	static void ConvertToText(const std::vector<std::string> &terms,
			std::vector<text::TextRef> &terms_text);
	static void ConvertToText(const std::vector<std::string> &terms,
			TTerms &terms_text);
	// Convert to text one string.
	static text::TextRef ConvertToText(const std::string &term);
	// Normalize string.
	static std::string Normalize(const std::string &term);
	// Normalize pattern. Remove all conditions and useless spaces.
	static std::string NormalizePattern(const std::string &pattern);
	// Checks, that full text equals to pattern. If true, that retruns
	// normalized text. Else returns "".
	static std::string GetNormalizedMatch(text::TextRef text,
			patterns::PatternRef pattern);
	static std::string GetNormalizedMatch(std::string text,
			patterns::PatternRef pattern);
	static std::string GetNormalizedMatch(text::TextRef text,
			patterns::PatternRef pattern,
			patterns::restrictions::AndRestriction &restriction);
	static std::string GetNormalizedMatch(std::string text,
			patterns::PatternRef pattern,
			patterns::restrictions::AndRestriction &restriction);
	// Change pattern with set of words.
	static std::string BuildPattern(const std::string &pattern,
			const std::map<std::string, std::string> &words);
	// Load patterns with alternates after #.
	static void LoadSimilarPatterns(
			const std::string &text, std::vector<std::string> &patterns,
			std::vector<std::vector<std::string> > &similar_patterns);
	static NamespaceRef BuildPatterns(const std::vector<std::string> &patterns,
			bool use_dictionary = true);
	// Extact *.st = *.st condition and remove it from pattern
	static std::string ExtractStCondition(std::string &pattern);
	// Extract variables from st condition.
	// Example: <A1.st = N2.st> will return <"A1", "N2">
	static std::pair<std::string, std::string>
		ConvertStCondition(const std::string &condition);

	static bool IsPretext(const std::string &word);
	static bool IsDelimiter(char symbol);
	static void ToUpper(std::string &word);

	static int LevinshteinDistance(const std::string &word1,
			const std::string &word2);
	static bool IsOrphologicalVariant(const std::string &term1,
			const std::string &term2);
};

class LSPL_EXPORT TTerm: public text::TextRef {
public:
	size_t count;
public:
	TTerm();
	TTerm(const text::TextRef &text) : text::TextRef(text), count(0) {
	}
	TTerm(const std::string &text) :
			text::TextRef(Util::ConvertToText(text)), count(0) {
	}
	TTerm(const text::TextRef &text, size_t count) :
			text::TextRef(text), count(count) {
	}
	TTerm(const std::string &text, size_t count) :
			text::TextRef(Util::ConvertToText(text)), count(count) {
	}
}; // class TTerm

} // namespace lspl.

#endif // __LSPL_UTIL

