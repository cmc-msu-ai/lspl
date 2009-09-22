/*
 * Author: Antonov Vadim (avadim@gmail.com)
 */

#include <algorithm>
#include <iostream>
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

		for(uint i = 0; i < patterns_namespace()->getPatternCount(); i++) {
			patterns::PatternRef pattern =
					patterns_namespace()->getPatternByIndex(i);
			text::MatchList matches = text()->getMatches(pattern);
			if (matches.size() != 0) {
				result.push_back(PatternsMatch(pattern, 0));
				std::cout << "Pattern: " << Util::out.convert(pattern->name) <<
						std::endl;
			}
		}

		std::sort(result.begin(), result.end(),
				ComparePatternsMatches);

		base::RangeSet range_set;
		for(int i = 0; i < result.size(); ++i) {
			patterns::PatternRef pattern = result[i].first;
			std::cout << " Pattern: " << Util::out.convert(pattern->name) << " " <<
					Util::out.convert(pattern->getSource()) << std::endl;
			text::MatchList matches = _text->getMatches(pattern);
			for(uint j = 0; j < matches.size(); j++) {
				text::MatchRef match = matches[j];
				base::Range range = match->getFragment(0);
				if (!range_set.FindRangeExtension(range)) {
					range_set.AddRange(range);
					++(result[i].second);
					std::cout << "Match '" <<
							Util::out.convert(match->getFragment(0).getText()) << "'" <<
							Util::out.convert(match->getFragment(0).getNormalizedText()) <<
							"'" << 
							Util::out.convert(match->getFragment(0).getPatternedText(0)) <<
							"'" << std::endl;
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
		return i.first->name.size() >= j.first->name.size();
	}
} // namespace lspl.
