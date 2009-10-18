 /*
  * Author: Antonov Vadim (avadim@gmail.com)
  */

#include <iostream>

#include "lspl/patterns/PatternBuilder.h"
#include "lspl/patterns/Pattern.h"
#include "lspl/text/readers/PlainTextReader.h"
#include "lspl/transforms/Normalization.h"
#include "SimilarityRecognizer.h"
#include "Util.h"

namespace lspl {

NamespaceRef SimilarityRecognizer::patterns_namespace() const {
	return _patterns_namespace;
}

std::vector<NamespaceRef> SimilarityRecognizer::similar_patterns_namespaces() 
		const {
	return _similar_patterns_namespaces;
}

void SimilarityRecognizer::LoadSimilarPatterns(const char *file) {
	_patterns_namespace = new Namespace();
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
		const std::vector<std::string> &terms2) const {
	std::vector<text::TextRef> terms1_text;
	std::vector<text::TextRef> terms2_text;
	Util::ConvertToText(terms1, terms1_text);
	Util::ConvertToText(terms2, terms2_text);
	transforms::Normalization normalization;
	/// terms1 && main patterns
	for(int j = 0; j < patterns_namespace()->getPatternCount(); ++j) {
		patterns::PatternRef pattern =
				patterns_namespace()->getPatternByIndex(j);

		for(int i = 0; i < terms1_text.size(); ++i) {
			text::MatchList matches = terms1_text[i]->getMatches(pattern);
			//std::cout << "1 pattern: " << " " << pattern->getSource() << std::endl;

			if (matches.size() == 1 && Util::CountWords(terms1[i]) == 
					Util::CountWords(matches[0]->getFragment(0).getText())){
				std::map<std::string, std::string> pattern_words;

				//std::cout << Util::out.convert(terms1[i]) << std::endl;
				//std::cout << Util::out.convert(pattern->getSource()) << std::endl;
				if (!Util::BuildWordsByPattern(
						normalization.normalize(matches[0]->getVariants().at(0)), pattern,
						pattern_words)) {
					continue;
				}

				FindSimilars(terms1[i], pattern_words, terms2_text,
						similar_patterns_namespaces()[j]);
			}
		}
	}
}

void SimilarityRecognizer::FindSimilars(
		const std::string &term1,
		std::map<std::string, std::string> &pattern_words,
		const std::vector<text::TextRef> &terms2_text,
		const NamespaceRef similar_patterns_namespace) const {
	transforms::Normalization normalization;
	for(int l = 0; l < similar_patterns_namespace->getPatternCount(); ++l) {
		patterns::PatternRef similar_pattern =
				similar_patterns_namespace->getPatternByIndex(l);
		for(int k = 0; k < terms2_text.size(); ++k) {
			//std::cout << "2 pattern: " << " " << similar_pattern->getSource() << std::endl;
			text::MatchList similar_matches =
					terms2_text[k]->getMatches(similar_pattern);

			if (similar_matches.size() == 1 &&
					Util::CountWords(terms2_text[k]->getContent()) == 
					Util::CountWords(similar_matches[0]->getFragment(0).getText())) {
				std::map<std::string, std::string> similar_pattern_words;

				if (
						!Util::BuildWordsByPattern(
						normalization.normalize(similar_matches[0]->getVariants().at(0)),
						similar_pattern, similar_pattern_words)) {
					continue;
				}

				if (IsSimilar(pattern_words, similar_pattern_words)) {
					std::cout << "Similar: " << Util::out.convert(term1) << " && " <<
							Util::out.convert(terms2_text[k]->getContent()) << std::endl;
				}

			}
		}
	}
}

bool SimilarityRecognizer::IsSimilar(std::map<std::string, std::string> &term1,
		std::map<std::string, std::string> &term2) const {
	for(std::map<std::string, std::string>::iterator i = term1.begin();
			i != term1.end(); ++i) {
		//std::cout << i->first << " " << Util::out.convert(i->second) << std::endl;
		if (term2.find(i->first) != term2.end() && term2[i->first] != i->second) {
			return false;
		}
	}
	return true;
}

} // namespace lspl
