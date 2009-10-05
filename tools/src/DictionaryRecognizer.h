/*
 * Author: Antonov Vadim (avadim@gmail.com)
 *
 * This class searches patterns from the dictionary in the text.
 */

#ifndef __LSPL_DICITONARY_RECOGNIZER
#define __LSPL_DICITONARY_RECOGNIZER

#include <vector>
#include <string>

#include "lspl/base/Base.h"
#include "lspl/base/RefCountObject.h"
#include "lspl/base/RefCountPtr.h"
#include "lspl/Namespace.h"
#include "lspl/patterns/Pattern.h"
#include "lspl/text/Text.h"

namespace lspl {

LSPL_REFCOUNT_FORWARD(PatternMatch);

class PatternMatch : public base::RefCountObject<PatternMatch> {
 public:
	patterns::PatternRef pattern;
	text::MatchList matches;
	std::string normalized_name;
	uint match_count;
 public:
	PatternMatch(const std::string &name,
			const patterns::PatternRef base_pattern);
};

typedef base::Reference<PatternMatch> PatternMatchRef;

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
	std::vector<PatternMatchRef> RecognizeAndSearch() const;

	// Compare function for the std::sort for patterns sorting.
	static bool ComparePatternsMatches(const PatternMatchRef &i,
			const PatternMatchRef &j);
};

} // namespace lspl.

#endif // __LSPL_DICITONARY_RECOGNIZER
