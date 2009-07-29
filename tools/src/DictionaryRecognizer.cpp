/*
 * Author: Antonov Vadim (avadim@gmail.com)
 */

#include <iostream>
#include <fstream>
#include <string>

#include "DictionaryRecognizer.h"
#include "lspl/patterns/PatternBuilder.h"
#include "lspl/text/Match.h"
#include "lspl/text/readers/PlainTextReader.h"
#include "lspl/utils/Conversion.h"


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

	std::list<std::pair<lspl::patterns::PatternRef, int> >
			DictionaryRecognizer::RecognizeAndSearch() const {
		std::list<std::pair<lspl::patterns::PatternRef, int> > result;

		for(uint i = 0; i < _patterns_namespace->getPatternCount(); i++) {
			lspl::patterns::PatternRef pattern = _patterns_namespace->getPatternByIndex(i);
			lspl::text::MatchList matches = _text->getMatches(*pattern);
			if (matches.size() != 0) {
				//std::cout << std::endl << "Pattern: " << out.convert( pt->name ) << std::endl;
				for(uint j = 0; j < matches.size(); j++) {
					lspl::text::MatchRef match = matches[j];
					lspl::base::Range range = match->getFragment(0);
					//std::cout << "Match '" << out.convert( match->getFragment(0).getText() ) << std::endl;
				}
			}
		}

		return result;
	}

} // namespace lspl.
