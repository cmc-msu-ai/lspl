 /*
  * Author: Antonov Vadim (avadim@gmail.com)
  */

#include <iostream>
#include <string>

#include "lspl/patterns/PatternBuilder.h"
#include "lspl/patterns/Pattern.h"
#include "lspl/text/readers/PlainTextReader.h"
#include "lspl/text/Text.h"
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
	/// terms1 && main patterns
	for(int j = 0; j < patterns_namespace()->getPatternCount(); ++j) {
		patterns::PatternRef pattern =
				patterns_namespace()->getPatternByIndex(j);
		text::TextRef pattern_text = Util::ConvertToText(pattern->getSource());
		for(int i = 0; i < terms1_text.size(); ++i) {
			text::MatchList matches = terms1_text[i]->getMatches(pattern);
			if (matches.size() == 1 &&
					terms1_text[i]->getWords().size() == 
							Util::CountWords(matches[0]->getFragment(0).getText())) {
				std::map<std::string, std::string> pattern_words;
				for(int l = 0; l < terms1_text[i]->getWords().size(); ++l) {
					text::markup::Word word = *(pattern_text->getWords()[l]);
					pattern_words[word.getBase()] =
							terms1_text[i]->getWords()[l]->getToken();
				}
				/// terms2 && similar_patterns
				for(int l = 0;
						l < similar_patterns_namespaces()[j]->getPatternCount(); ++l) {
					patterns::PatternRef similar_pattern = 
							similar_patterns_namespaces()[j]->getPatternByIndex(l);
					text::TextRef similar_pattern_text =
							Util::ConvertToText(similar_pattern->getSource());
					for(int k = 0; k < terms2_text.size(); ++k) {
						text::MatchList similar_matches =
								terms2_text[k]->getMatches(similar_pattern);
						if (similar_matches.size() == 1 &&
								terms2_text[k]->getWords().size() ==
										Util::CountWords(
												similar_matches[0]->getFragment(0).getText())) {
							bool is_similar = true;
							for(int n = 0; n < terms2_text[k]->getWords().size(); ++n) {
								if (pattern_words.find(similar_pattern_text->getWords()[n]->getBase()) !=
												pattern_words.end() &&
										pattern_words[similar_pattern_text->getWords()[n]->getBase()] !=
												terms2_text[k]->getWords()[n]->getToken()) {
									is_similar = false;
									break;
								}
							}
							if (is_similar) {
								std::cout << "Similar: " << terms1[i] << " && " << terms2[k] <<
										std::endl;
							}
						}
					}
				}
			}
		}
	}
}


} // namespace lspl
