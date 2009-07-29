/*
 * Author: Antonov Vadim (avadim@gmail.com)
 *
 * This class searches patterns from the dictionary in the text.
 */

#ifndef __LSPL_DICITONARY_RECONGNIZER
#define __LSPL_DICITONARY_RECONGNIZER

#include <list>

#include "lspl/Namespace.h"
#include "lspl/text/Text.h"

namespace lspl {

class DictionaryRecognizer {
 private:
	lspl::NamespaceRef _patterns_namespace;
	lspl::text::TextRef _text;

	// Read text from the file, convert it from the UTF8 to the WIN-1251 encoding.
	std::string LoadTextFromFile(const char* filename) const;
	// Load patterns from the file and convert them into the lspl representation.
	lspl::NamespaceRef LoadPatterns(const char* dictionary_filename) const;
	// Load text from the file and convert it into the lspl representation.
	lspl::text::TextRef LoadText(const char* text_filename) const;
 public:
	DictionaryRecognizer(const char* dictionary_filename,
			const char* text_filename);

	DictionaryRecognizer(const lspl::NamespaceRef patterns_namespace,
			const lspl::text::TextRef text);

	std::list<std::pair<lspl::patterns::PatternRef, int> > 
			RecognizeAndSearch() const;
};

} // namespace lspl.

#endif // __LSPL_DICITONARY_RECONGNIZER
