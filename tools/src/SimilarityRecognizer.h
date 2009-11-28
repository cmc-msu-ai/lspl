/*
 * Author: Antonov Vadim (avadim@gmail.com)
 *
 * This class represents similarity properties.
 */

#ifndef __LSPL_SIMILARITYRECOGNIZER
#define __LSPL_SIMILARITYRECOGNIZER

#include <map>
#include <string>
#include <vector>

#include "lspl/Namespace.h"
#include "lspl/text/Text.h"
#include "SynDictionary.h"

namespace lspl {

class SimilarityRecognizer {
 private:
	class SimilarFinder {
	 private:
		const std::vector<text::TextRef> &_terms1;
		const std::vector<text::TextRef> &_terms2;
		NamespaceRef _patterns_namespace;
		std::vector<std::vector<std::string> > &_similar_patterns;
		dictionaries::SynDictionary _synonim_dictionary;

		const std::vector<text::TextRef> &terms1() const;
		const std::vector<text::TextRef> &terms2() const;
		NamespaceRef patterns_namespace() const;
		std::vector<std::vector<std::string> > &similar_patterns() const;
		const dictionaries::SynDictionary &synonim_dictionary() const;

		std::vector<int> *FindSimilars(const text::TextRef term1,
				std::map<std::string, std::string> &pattern_words,
				const std::vector<std::string> &similar_patterns_namespace);

		bool IsSimilar(std::map<std::string, std::string> &term1,
				std::map<std::string, std::string> &term2) const;
	 public:
		SimilarFinder(const std::vector<text::TextRef> &terms1,
				const std::vector<text::TextRef> &terms2,
				NamespaceRef patterns_namespace,
				std::vector<std::vector<std::string> > &similar_patterns);

		std::vector<std::vector<int> *> *FindSimilars();
	};

	NamespaceRef _patterns_namespace;
	std::vector<std::vector<std::string> > _similar_patterns;

	NamespaceRef patterns_namespace() const;
	std::vector<std::vector<std::string> > &similar_patterns();
	void LoadSimilarPatterns(const char *file);
 public:
	SimilarityRecognizer(const char *similarity_patterns_file);

	void FindSimilars(const std::vector<std::string> &terms1,
			const std::vector<std::string> &terms2);

	void FindSimilars(const std::vector<text::TextRef> &terms1,
			const std::vector<text::TextRef> &terms2);
};

} // namespace lspl.

#endif // __LSPL_SIMILARITYRECOGNIZER

