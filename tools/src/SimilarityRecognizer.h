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
#include "Util.h"

namespace lspl {

typedef std::set<int> TResult;
typedef std::vector<boost::shared_ptr<TResult> > TResults;
typedef std::vector<NamespaceRef> TSimilarPatterns;
typedef std::pair<patterns::matchers::Variable,
	patterns::matchers::Variable> TStCondition;
typedef std::vector<TStCondition *> TStConditionsForSimilarPatterns;
typedef std::vector<TStConditionsForSimilarPatterns> TStConditions;
typedef std::map<std::string, std::string> TTermWords;

class SimilarityRecognizer {
 private:
	class SimilarFinder {
	 private:
		const TTerms &_terms1;
		const TTerms &_terms2;
		NamespaceRef _patterns_namespace;
		TSimilarPatterns &_similar_patterns;
		const TStConditions &_st_conditions;
		dictionaries::SynDictionary _synonim_dictionary;

		const TTerms &terms1() const;
		const TTerms &terms2() const;
		NamespaceRef patterns_namespace() const;
		TSimilarPatterns &similar_patterns() const;
		const dictionaries::SynDictionary &synonim_dictionary() const;

		void FindSimilars(const TTerm &term1,
				TTermWords &pattern_words,
				NamespaceRef similar_patterns_namespace,
				const TStConditionsForSimilarPatterns &st_conditions,
				patterns::matchers::Context &context,
				TResult &result);

		void FindEqualWords(TResults &result) const;
		bool IsSimilar(TTermWords &term1, TTermWords &term2) const;
		bool IsSimilar(const TTerm &term1, const TTerm &term2) const;

		boost::shared_ptr<patterns::restrictions::AndRestriction>
				GenerateAndRestriction(const TTermWords &pattern_words);
	 public:
		SimilarFinder(const TTerms &terms1, const TTerms &terms2,
				NamespaceRef patterns_namespace,
				TSimilarPatterns &similar_patterns,
				const TStConditions &st_conditions);

		boost::shared_ptr<TResults> FindSimilars();
	};

	NamespaceRef _patterns_namespace;
	TSimilarPatterns _similar_patterns;
	TStConditions _st_conditions;

	NamespaceRef patterns_namespace() const;
	TSimilarPatterns &similar_patterns();
	void LoadSimilarPatterns(const char *file);
 public:
	SimilarityRecognizer(const char *similarity_patterns_file);

	void FindSimilars(const std::vector<std::string> &terms1,
			const std::vector<std::string> &terms2);

	void FindSimilars(const TTerms &terms1, const TTerms &terms2);
};

} // namespace lspl.

#endif // __LSPL_SIMILARITYRECOGNIZER

