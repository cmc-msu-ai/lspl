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

namespace lspl {

class SimilarityRecognizer {
 private:
	enum LoadSimilarPatternsState {
		LOADSTATE_NEW,
		LOADSTATE_SIMILARS
	};
	class SimilarFinder {
	 private:
		const std::vector<text::TextRef> &_terms1;
		const std::vector<text::TextRef> &_terms2;
		NamespaceRef _patterns_namespace;
		std::vector<NamespaceRef> &_similar_patterns_namespaces;

		const std::vector<text::TextRef> &terms1() const;
		const std::vector<text::TextRef> &terms2() const;
		NamespaceRef patterns_namespace() const;
		std::vector<NamespaceRef> &similar_patterns_namespaces() const;

		std::vector<int> *FindSimilars(const text::TextRef term1,
				std::map<std::string, std::string> &pattern_words,
				const NamespaceRef similar_patterns_namespace);

		bool IsSimilar(std::map<std::string, std::string> &term1,
				std::map<std::string, std::string> &term2) const;
	 public:
		SimilarFinder(const std::vector<text::TextRef> &terms1,
				const std::vector<text::TextRef> &terms2,
				NamespaceRef patterns_namespace,
				std::vector<NamespaceRef> &similar_patterns_namespaces);

		std::vector<std::vector<int> *> *FindSimilars();
	};

	NamespaceRef _patterns_namespace;
	std::vector<NamespaceRef> _similar_patterns_namespaces;

	NamespaceRef patterns_namespace() const;
	std::vector<NamespaceRef> &similar_patterns_namespaces();
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

