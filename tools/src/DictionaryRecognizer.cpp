/*
 * Author: Antonov Vadim (avadim@gmail.com)
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include "DictionaryRecognizer.h"
#include "lspl/patterns/PatternBuilder.h"
#include "lspl/text/Match.h"
#include "lspl/text/readers/PlainTextReader.h"
#include "lspl/utils/Conversion.h"
#include "RangeSet.h"


namespace lspl {
	static lspl::utils::Conversion in("UTF-8", "CP1251");
	static lspl::utils::Conversion out("CP1251", "UTF-8");

	std::string DictionaryRecognizer::LoadTextFromFile(const char* filename)
			const {
		std::ifstream input_stream(filename);
		input_stream.seekg(0, std::ios_base::end);
		int size = input_stream.tellg();
		input_stream.seekg(0, std::ios_base::beg);
		char* buffer = new char[size];
		input_stream.read(buffer, size);
		std::string result = in.convert(buffer, size);
		delete [] buffer;
		return result;
	}

	lspl::NamespaceRef
			DictionaryRecognizer::LoadPatterns(const char* dictionary_filename)
			const {
		lspl::NamespaceRef patterns_namespace = new lspl::Namespace();
		lspl::patterns::PatternBuilderRef builder =
				new lspl::patterns::PatternBuilder(patterns_namespace);
		uint startIndex = 0;
		std::string text = LoadTextFromFile(dictionary_filename);
		for(uint i = 0; i < text.size(); i++)
			if ((i == text.size() - 1) || (text[i] == '\n')) {
				uint lineSize = i - startIndex;
				if (lineSize > 1) {
					std::string line(text, startIndex, lineSize);
					try {
						builder->build(line);
					}	catch(lspl::patterns::PatternBuildingException) {
					}
				}
				startIndex = i + 1;
			}
		return patterns_namespace;
	}

	lspl::text::TextRef DictionaryRecognizer::LoadText(const char* text_filename)
			const {
		std::string text = LoadTextFromFile(text_filename);
		lspl::text::readers::PlainTextReader reader;
		return reader.readFromString(text);
	}

	DictionaryRecognizer::DictionaryRecognizer(const char* dictionary_filename,
			const char* text_filename) {
		_patterns_namespace = LoadPatterns(dictionary_filename);
		_text = LoadText(text_filename);
	}

	DictionaryRecognizer::DictionaryRecognizer(
			const lspl::NamespaceRef patterns_namespace,
			const lspl::text::TextRef text) {
		_patterns_namespace = patterns_namespace;
		_text = text;
	}

	std::vector<std::pair<lspl::patterns::PatternRef, int> >
			DictionaryRecognizer::RecognizeAndSearch() const {
		std::vector<std::pair<lspl::patterns::PatternRef, int> > result;

		for(uint i = 0; i < _patterns_namespace->getPatternCount(); i++) {
			lspl::patterns::PatternRef pattern =
					_patterns_namespace->getPatternByIndex(i);
			lspl::text::MatchList matches = _text->getMatches(pattern);
			if (matches.size() != 0) {
				result.push_back(
						std::pair<lspl::patterns::PatternRef, int>(pattern, 0));
				std::cout << "Pattern: " << out.convert( pattern->name ) << std::endl;
			}
		}

		std::sort(result.begin(), result.end(),
				ComparePatternsMatches);

		lspl::base::RangeSet range_set;
		for(int i = 0; i < result.size(); ++i) {
			lspl::patterns::PatternRef pattern = result[i].first;
			std::cout << " Pattern: " << out.convert( pattern->name ) << std::endl;
			lspl::text::MatchList matches = _text->getMatches(pattern);
			for(uint j = 0; j < matches.size(); j++) {
				lspl::text::MatchRef match = matches[j];
				lspl::base::Range range = match->getFragment(0);
				if (!range_set.FindRangeExtension(range)) {
					range_set.AddRange(range);
					++(result[i].second);
					std::cout << "Match '" <<
							out.convert( match->getFragment(0).getText() ) << "'" <<
							std::endl;
				} else {
					std::cout << "Don't include match '" <<
							out.convert( match->getFragment(0).getText() ) << "'" <<
							std::endl;
				}
			}
		}

		return result;
	}

	bool DictionaryRecognizer::ComparePatternsMatches(
				const std::pair<lspl::patterns::PatternRef, int> &i,
				const std::pair<lspl::patterns::PatternRef, int> &j) {
		return i.first->name.size() >= j.first->name.size();
	}
} // namespace lspl.
