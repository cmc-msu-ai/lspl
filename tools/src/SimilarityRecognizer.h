/*
 * Author: Antonov Vadim (avadim@gmail.com)
 *
 * This class represents common utils.
 */

#ifndef __LSPL_SIMILARITYRECOGNIZER
#define __LSPL_SIMILARITYRECOGNIZER

#include <string>
#include <vector>

#include "lspl/Namespace.h"

namespace lspl {

class SimilarityRecognizer {
 private:
	enum LoadSimilarPatternsState {
		LOADSTATE_NEW,
		LOADSTATE_SIMILARS
	};
	NamespaceRef _patterns_namespace;
	std::vector<NamespaceRef> _similar_patterns_namespaces;

	NamespaceRef patterns_namespace() const;
	std::vector<NamespaceRef> similar_patterns_namespaces() const;
	void LoadSimilarPatterns(const char *file);
 public:
	SimilarityRecognizer(const char *similarity_patterns_file);

	void FindSimilars(const std::vector<std::string> &terms1,
			const std::vector<std::string> &terms2) const;
};

} // namespace lspl.

#endif // __LSPL_SIMILARITYRECOGNIZER

