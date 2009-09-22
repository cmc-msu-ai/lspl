 /*
  * Author: Antonov Vadim (avadim@gmail.com)
  */

#include <iostream>
#include <string>

#include "lspl/patterns/PatternBuilder.h"
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

void SimilarityRecognizer::ConvertToText(const std::vector<std::string> &terms,
		std::vector<text::TextRef> &terms_text) const {
	text::readers::PlainTextReader reader;
	for(int i = 0; i < terms.size(); ++i) {
		terms_text.push_back(reader.readFromString(terms[i]));
	}
}

void SimilarityRecognizer::FindSimilars(const std::vector<std::string> &terms1,
		const std::vector<std::string> &terms2) const {
	std::vector<text::TextRef> terms1_text;
	std::vector<text::TextRef> terms2_text;
	ConvertToText(terms1, terms1_text);
	ConvertToText(terms2, terms2_text);
	for(int i = 0; i < terms1_text.size(); ++i) {
		for(int j = 0; j < patterns_namespace()->getPatternCount(); ++j) {
			patterns::PatternRef pattern =
					patterns_namespace()->getPatternByIndex(j);
			text::MatchList matches = terms1_text[i]->getMatches(pattern);
			if (matches.size() == 1 &&
					terms1_text[i]->getWords().size() == 1 /* pattern-> */) {
				for(int k = 0; k < terms2_text.size(); ++k) {
					for(int l = 0;
							l < similar_patterns_namespaces()[j]->getPatternCount(); ++l) {
						patterns::PatternRef similar_pattern = 
								similar_patterns_namespaces()[j]->getPatternByIndex(l);
					}
				}
			}
		}
	}
}

} // namespace lspl
