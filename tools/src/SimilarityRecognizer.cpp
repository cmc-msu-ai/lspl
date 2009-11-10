 /*
  * Author: Antonov Vadim (avadim@gmail.com)
  */

#include <iostream>
#include <boost/scoped_ptr.hpp>

#include "lspl/patterns/PatternBuilder.h"
#include "lspl/patterns/Pattern.h"
#include "lspl/text/readers/PlainTextReader.h"
#include "SimilarityRecognizer.h"
#include "SynDictionary.h"
#include "Util.h"

namespace lspl {

transforms::Normalization SimilarityRecognizer::SimilarFinder::normalization;

SimilarityRecognizer::SimilarFinder::SimilarFinder(
		const std::vector<text::TextRef> &terms1,
		const std::vector<text::TextRef> &terms2,
		NamespaceRef patterns_namespace,
		std::vector<NamespaceRef> &similar_patterns_namespaces) :
		_terms1(terms1),
		_terms2(terms2),
		_patterns_namespace(patterns_namespace),
		_similar_patterns_namespaces(similar_patterns_namespaces) {
}

const std::vector<text::TextRef>
		&SimilarityRecognizer::SimilarFinder::terms1() const {
	return _terms1;
}

const std::vector<text::TextRef>
		&SimilarityRecognizer::SimilarFinder::terms2() const {
	return _terms2;
}

NamespaceRef SimilarityRecognizer::SimilarFinder::patterns_namespace() const {
	return _patterns_namespace;
}

std::vector<NamespaceRef>
		&SimilarityRecognizer::SimilarFinder::similar_patterns_namespaces() const {
	return _similar_patterns_namespaces;
}

std::vector<std::vector<int> *> *
		SimilarityRecognizer::SimilarFinder::FindSimilars() {
	std::vector<std::vector<int> *> *result =
			new std::vector<std::vector<int> *>();
	/// terms1 && main patterns
	for(int j = 0; j < patterns_namespace()->getPatternCount(); ++j) {
		patterns::PatternRef pattern =
				patterns_namespace()->getPatternByIndex(j);

		for(int i = 0; i < terms1().size(); ++i) {
			text::MatchList matches = terms1()[i]->getMatches(pattern);
			if (!matches.size()) {
				continue;
			}

			int terms_words_count = Util::CountWords(terms1()[i]->getContent());
			int match_words_count =
					Util::CountWords(matches[0]->getFragment(0).getText()); 
			if (matches.size() != 1  || terms_words_count != match_words_count ) {
				continue;
			}

			std::map<std::string, std::string> pattern_words;

			//std::cout << Util::out.convert(terms1[i]) << std::endl;
			//std::cout << Util::out.convert(pattern->getSource()) << std::endl;
			std::string normalized_match =
					normalization.normalize(matches[0]->getVariants().at(0)); 
			if (!Util::BuildWordsByPattern(normalized_match, pattern,
					pattern_words)) {
				continue;
			}

			result->push_back(FindSimilars(terms1()[i], pattern_words,
					similar_patterns_namespaces()[j]));
		}
	}
	return result;
}

std::vector<int> *SimilarityRecognizer::SimilarFinder::FindSimilars(
		const text::TextRef term1,
		std::map<std::string, std::string> &pattern_words,
		const NamespaceRef similar_patterns_namespace) {
	std::vector<int> *result = new std::vector<int>();
	for(int l = 0; l < similar_patterns_namespace->getPatternCount(); ++l) {
		patterns::PatternRef similar_pattern =
				similar_patterns_namespace->getPatternByIndex(l);
		for(int k = 0; k < terms2().size(); ++k) {
			text::MatchList similar_matches =
					terms2()[k]->getMatches(similar_pattern);
			if (!similar_matches.size()) {
				continue;
			}

			int terms_words_count = Util::CountWords(terms2()[k]->getContent());
			int match_words_count =
					Util::CountWords(similar_matches[0]->getFragment(0).getText());
			if (similar_matches.size() != 1 ||
					terms_words_count != match_words_count) {
				continue;
			}
			std::map<std::string, std::string> similar_pattern_words;

			std::string normalized_match =
					normalization.normalize(similar_matches[0]->getVariants().at(0));
			if (!Util::BuildWordsByPattern(normalized_match, similar_pattern,
					similar_pattern_words)) {
				continue;
			}

			if (IsSimilar(pattern_words, similar_pattern_words)) {
				std::cout << "Similar: " << Util::out.convert(term1->getContent()) << " && " <<
						Util::out.convert(terms2()[k]->getContent()) << std::endl;
				result->push_back(k);
			}
		}
	}
	return result;
}

bool SimilarityRecognizer::SimilarFinder::IsSimilar(
		std::map<std::string, std::string> &term1,
		std::map<std::string, std::string> &term2) const {
	for(std::map<std::string, std::string>::iterator i = term1.begin();
			i != term1.end(); ++i) {
		if (term2.find(i->first) != term2.end() && term2[i->first] != i->second) {
			return false;
		}
	}
	return true;
}

NamespaceRef SimilarityRecognizer::patterns_namespace() const {
	return _patterns_namespace;
}

std::vector<NamespaceRef> &SimilarityRecognizer::similar_patterns_namespaces() {
	return _similar_patterns_namespaces;
}

void SimilarityRecognizer::LoadSimilarPatterns(const char *file) {
	_patterns_namespace = new Namespace();
	_patterns_namespace->addDictionary(
			new dictionaries::SynDictionary("Syn", ""));
	patterns::PatternBuilderRef builder =
			new patterns::PatternBuilder(_patterns_namespace);
	patterns::PatternBuilderRef similar_builder;
	std::string text = Util::LoadTextFromFile(file);
	uint start_index = 0;
	LoadSimilarPatternsState state = LOADSTATE_NEW;

	for(int i = 0; i < text.size(); ++i) {
		if (i == text.size() - 1 || text[i] == '\n') {
			uint line_size = i - start_index;
			std::string line(text, start_index, line_size);
			line = Util::Trim(line);
			if (line.size() >= 1) {
				switch (state) {
					case LOADSTATE_NEW:
						if (line[line.size() - 1] == '#') {
							try {
								std::cout << "new build pattern " << 
										line.substr(0, line.size() - 1) << std::endl;
								builder->build(line.substr(0, line.size() - 1));
								_similar_patterns_namespaces.push_back(new Namespace());
								_similar_patterns_namespaces[
										_similar_patterns_namespaces.size() - 1]->addDictionary(
												new dictionaries::SynDictionary("Syn", ""));
								similar_builder = new patterns::PatternBuilder(
										_similar_patterns_namespaces[
												_similar_patterns_namespaces.size() - 1]);
								state = LOADSTATE_SIMILARS;
							} catch (patterns::PatternBuildingException) {
								std::cout << "Failed to build pattern " << line << std::endl;
							}
						}
						break;
					case LOADSTATE_SIMILARS:
						if (line[line.size() - 1] != ',') {
							try {
								similar_builder->build(line);
								std::cout << "last build pattern " << line << std::endl;
							} catch (patterns::PatternBuildingException) {
								std::cout << "Failed to build pattern " << line << std::endl;
							}
							state = LOADSTATE_NEW;
						} else {
							try {
								similar_builder->build(line.substr(0, line.size() - 1));
								std::cout << "build pattern " << line << std::endl;
							} catch (patterns::PatternBuildingException) {
								std::cout << "Failed to build pattern " << line << std::endl;
							}
						}
						break;
				}
			}
			start_index = i + 1;
		}
	}
}

SimilarityRecognizer::SimilarityRecognizer(
		const char *similarity_patterns_file) {
	LoadSimilarPatterns(similarity_patterns_file);
}

void SimilarityRecognizer::FindSimilars(const std::vector<std::string> &terms1,
		const std::vector<std::string> &terms2) {
	std::vector<text::TextRef> terms1_text;
	std::vector<text::TextRef> terms2_text;
	Util::ConvertToText(terms1, terms1_text);
	Util::ConvertToText(terms2, terms2_text);
	FindSimilars(terms1_text, terms2_text);
}

void SimilarityRecognizer::FindSimilars(
		const std::vector<text::TextRef> &terms1,
		const std::vector<text::TextRef> &terms2) {
	boost::scoped_ptr<SimilarFinder> similar_finder(new SimilarFinder(terms1,
			terms2,	patterns_namespace(), similar_patterns_namespaces()));
	similar_finder->FindSimilars();
}

} // namespace lspl
