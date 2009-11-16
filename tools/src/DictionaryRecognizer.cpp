/*
 * Author: Antonov Vadim (avadim@gmail.com)
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <string>

#include "DictionaryRecognizer.h"
#include "lspl/base/BaseInternal.h"
#include "lspl/patterns/PatternBuilder.h"
#include "lspl/text/Loop.h"
#include "lspl/text/Match.h"
#include "lspl/transforms/Normalization.h"
#include "RangeSet.h"
#include "SynDictionary.h"
#include "Util.h"

LSPL_REFCOUNT_CLASS( lspl::PatternMatch );

namespace lspl {
	PatternMatch::PatternMatch(const std::string &name,
			const patterns::PatternRef base_pattern) : match_count(0),
			normalized_name(name), pattern(base_pattern) {
	}

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
		/*patterns_namespace->addDictionary(
				new dictionaries::SynDictionary("Syn", ""));*/
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

	std::vector<PatternMatchRef> DictionaryRecognizer::RecognizeAndSearch()
			const {
		std::vector<PatternMatchRef> result;
		std::vector<std::string> patterns;
		std::map<std::string, PatternMatchRef> result_matches;

		transforms::Normalization normalization;

		std::cout << "Transitions: " << text::Transition::aliveObjectsCount << std::endl;

		for(uint i = 0; i < patterns_namespace()->getPatternCount(); i++) {
			patterns::PatternRef pattern = patterns_namespace()->getPatternByIndex(i);

			std::cout << "Pattern: " << Util::out.convert(pattern->name) << std::endl;

			text::MatchList matches = text()->getMatches(pattern);

			if (/*pattern->name != "MSP"pattern->getSource()*/!matches.size()) {
				//std::cout << "Pattern: " << Util::out.convert(pattern->name) <<
				//		std::endl;
				std::cout << "No matches" << std::endl;
				continue;
			} else {
				std::cout << "Matches: " << matches.size() << std::endl;
				std::cout << "Deps:";
				foreach( const patterns::Pattern * d, pattern->getDependencies() )
					std::cout << " " << d->name;
				std::cout << std::endl;

				std::cout << "Transitions: " << text::Transition::aliveObjectsCount << std::endl;
			}
			for(uint j = 0; j < matches.size(); ++j) {
				text::MatchRef match = matches[j];
				std::string match_s = match->getFragment(0).getText(); 
				/*std::cout << "Match '" <<
							Util::out.convert(match->getFragment(0).getText()) << "'" <<
							std::endl;*/
				std::string normalized_match =
						normalization.normalize(match->getVariants().at(0));
				if (result_matches.find(normalized_match) == result_matches.end()) {
					PatternMatchRef patterns_match =
							new PatternMatch(normalized_match, pattern);
					patterns.push_back(normalized_match);
					result_matches[normalized_match] = patterns_match;
				}
				result_matches[normalized_match]->matches.push_back(match);
			}
		}

		std::sort(patterns.begin(), patterns.end(), ComparePatternsMatches);

		base::RangeSet range_set;
		for(uint i = 0; i < patterns.size(); ++i) {
			std::string normalized_match = patterns[i];
			PatternMatchRef pattern = result_matches[normalized_match];
			//std::cout << "Term: " << Util::out.convert(normalized_match) << std::endl;
			result.push_back(pattern);
			for(uint j = 0; j < pattern->matches.size(); j++) {
				text::MatchRef match = pattern->matches[j];
				base::Range range = match->getFragment(0);
				if (!range_set.FindRangeExtension(range)) {
					range_set.AddRange(range);
					++(pattern->match_count);
					/*std::cout << "Match '" <<
							Util::out.convert(match->getFragment(0).getText()) << "'" <<
							std::endl;*/
				} else {
					/*std::cout << "Don't include match '" <<
							Util::out.convert(match->getFragment(0).getText()) << "'" <<
							std::endl;*/
				}
			}
		}

		return result;
	}

	bool DictionaryRecognizer::ComparePatternsMatches(const std::string &i,
			const std::string &j) {
		return i.size() > j.size();
	}
} // namespace lspl.
