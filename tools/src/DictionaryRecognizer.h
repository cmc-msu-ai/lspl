/*
 * Author: Antonov Vadim (avadim@gmail.com)
 *
 * This class searches patterns from the dictionary in the text.
 */

#ifndef __LSPL_DICITONARY_RECOGNIZER
#define __LSPL_DICITONARY_RECOGNIZER

#include <vector>
#include <string>

#include "lspl/Namespace.h"
#include "lspl/text/Text.h"

namespace lspl {

typedef std::pair<std::string, int> PatternsMatch;

class DictionaryRecognizer {
 private:
	NamespaceRef _patterns_namespace;
	text::TextRef _text;

	NamespaceRef patterns_namespace() const;
	text::TextRef text() const;

	// Load patterns from the file and convert them into the lspl representation.
	NamespaceRef LoadPatterns(const char *dictionary_filename) const;
 public:
	DictionaryRecognizer(const char *dictionary_filename,
			const char *text_filename);

	DictionaryRecognizer(const NamespaceRef patterns_namespace,
			const char *text_filename);

	DictionaryRecognizer(const NamespaceRef patterns_namespace,
			const text::TextRef text);

	// Find dicrionary terms in the text.
	std::vector<PatternsMatch > RecognizeAndSearch() const;

	// Compare function for the std::sort for patterns sorting.
	static bool ComparePatternsMatches(const PatternsMatch &i,
			const PatternsMatch &j);
};

} // namespace lspl.

#endif // __LSPL_DICITONARY_RECOGNIZER
