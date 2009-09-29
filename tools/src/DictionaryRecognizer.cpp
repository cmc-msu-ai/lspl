/*
 * Author: Antonov Vadim (avadim@gmail.com)
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <string>

#include "DictionaryRecognizer.h"
#include "lspl/patterns/PatternBuilder.h"
#include "lspl/text/Match.h"
#include "RangeSet.h"
#include "Util.h"

namespace lspl {
	NamespaceRef DictionaryRecognizer::patterns_namespace() const {
		return _patterns_namespace;
	}

	text::TextRef DictionaryRecognizer::text() const {
		return _text;
	}

	NamespaceRef
			DictionaryRecognizer::LoadPatterns(const char* dictionary_filename)
			const {
		NamespaceRef patterns_namespace = new Namespace();
		patterns::PatternBuilderRef builder =
				new patterns::PatternBuilder(patterns_namespace);
		int start_index = 0;
		std::string text = Util::LoadTextFromFile(dictionary_filename);
		for(uint i = 0; i < text.size(); i++)
			if ((i == text.size() - 1) || (text[i] == '\n')) {
				uint line_size = i - start_index;
				if (line_size > 1) {
					std::string line(text, start_index, line_size);
					try {
						builder->build(line);
					}	catch(patterns::PatternBuildingException) {
					}
				}
				start_index = i + 1;
			}
		return patterns_namespace;
	}

	DictionaryRecognizer::DictionaryRecognizer(const char* dictionary_filename,
			const char* text_filename) {
		_patterns_namespace = LoadPatterns(dictionary_filename);
		_text = Util::LoadText(text_filename);
	}

	DictionaryRecognizer::DictionaryRecognizer(
			const NamespaceRef patterns_namespace,
			const char *text_filename) {
		_patterns_namespace = patterns_namespace;
		_text = Util::LoadText(text_filename);
	}

	DictionaryRecognizer::DictionaryRecognizer(
			const NamespaceRef patterns_namespace,
			const text::TextRef text) {
		_patterns_namespace = patterns_namespace;
		_text = text;
	}

	std::vector<PatternsMatch> DictionaryRecognizer::RecognizeAndSearch() const {
		std::vector<PatternsMatch> result;
		std::map<std::string, std::vector<text::MatchRef> > result_matches;

		for(uint i = 0; i < patterns_namespace()->getPatternCount(); i++) {
			patterns::PatternRef pattern =
					patterns_namespace()->getPatternByIndex(i);
			text::MatchList matches = text()->getMatches(pattern);
			for(uint j = 0; j < matches.size(); ++j) {
				std::string normalized_match =
						matches[j]->getVariants().at(0).at(3).cast<Words>().getBase();
						//Util::Normalize(matches[j]->getFragment(0).getText());
				if (result_matches.find(normalized_match) == result_matches.end()) {
					result.push_back(PatternsMatch(normalized_match, 0));
				}
				result_matches[normalized_match].push_back(matches[j]);
			}
			if (matches.size() != 0) {
				//result.push_back(PatternsMatch(pattern, 0));
				std::cout << "Pattern: " << Util::out.convert(pattern->name) <<
						std::endl;
			}
		}

		std::sort(result.begin(), result.end(),
				ComparePatternsMatches);

		base::RangeSet range_set;
		for(uint i = 0; i < result.size(); ++i) {
			std::string normalized_match = result[i].first;
			std::cout << "Term: " << Util::out.convert(normalized_match) << std::endl;
			for(uint j = 0; j < result_matches[normalized_match].size(); j++) {
				text::MatchRef match = result_matches[normalized_match][j];
				base::Range range = match->getFragment(0);
				if (!range_set.FindRangeExtension(range)) {
					range_set.AddRange(range);
					++(result[i].second);
					std::cout << "Match '" <<
							Util::out.convert(match->getFragment(0).getText()) << "'" <<
							std::endl;
				} else {
					std::cout << "Don't include match '" <<
							Util::out.convert(match->getFragment(0).getText()) << "'" <<
							std::endl;
				}
			}
		}

		return result;
	}

	bool DictionaryRecognizer::ComparePatternsMatches(const PatternsMatch &i,
			const PatternsMatch &j) {
		return i.first.size() >= j.first.size();
	}
} // namespace lspl.
