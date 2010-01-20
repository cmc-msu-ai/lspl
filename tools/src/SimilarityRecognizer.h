/*
 * Author: Antonov Vadim (avadim@gmail.com)
 *
 * This class represents similarity properties.
 */

#ifndef __LSPL_SIMILARITYRECOGNIZER
#define __LSPL_SIMILARITYRECOGNIZER

#include <map>
#include <set>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "lspl/Namespace.h"
#include "lspl/text/Text.h"
#include "lspl/patterns/matchers/Variable.h"
#include "lspl/patterns/restrictions/AndRestriction.h"
#include "SynDictionary.h"

namespace lspl {

class SimilarityRecognizer {
 private:
	class SimilarFinder {
	 private:
		const std::vector<text::TextRef> &_terms1;
		const std::vector<text::TextRef> &_terms2;
		NamespaceRef _patterns_namespace;
		std::vector<NamespaceRef> &_similar_patterns;
		const std::vector<std::vector<std::pair<patterns::matchers::Variable,
				patterns::matchers::Variable> *> > &_st_conditions;
		dictionaries::SynDictionary _synonim_dictionary;

		const std::vector<text::TextRef> &terms1() const;
		const std::vector<text::TextRef> &terms2() const;
		NamespaceRef patterns_namespace() const;
		std::vector<NamespaceRef> &similar_patterns() const;
		const dictionaries::SynDictionary &synonim_dictionary() const;

		std::vector<int> *FindSimilars(const text::TextRef term1,
				std::map<std::string, std::string> &pattern_words,
				NamespaceRef similar_patterns_namespace,
				const std::vector<std::pair<patterns::matchers::Variable,
						patterns::matchers::Variable> *> &st_conditions,
				patterns::matchers::Context &context);

		bool IsSimilar(std::map<std::string, std::string> &term1,
				std::map<std::string, std::string> &term2) const;
		bool IsSimilar(const std::string &term1,
				const std::string &term2) const;

		boost::shared_ptr<patterns::restrictions::AndRestriction>
				GenerateAndRestriction(
						const std::map<std::string, std::string> &pattern_words);
	 public:
		SimilarFinder(const std::vector<text::TextRef> &terms1,
				const std::vector<text::TextRef> &terms2,
				NamespaceRef patterns_namespace,
				std::vector<NamespaceRef> &similar_patterns,
				const std::vector<std::vector<std::pair<patterns::matchers::Variable,
						patterns::matchers::Variable> *> > &st_conditions);

		std::vector<std::set<int> *> *FindSimilars();
	};

	NamespaceRef _patterns_namespace;
	std::vector<NamespaceRef> _similar_patterns;
	std::vector<std::vector<std::pair<patterns::matchers::Variable,
			patterns::matchers::Variable> *> > _st_conditions;

	NamespaceRef patterns_namespace() const;
	std::vector<NamespaceRef> &similar_patterns();
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

