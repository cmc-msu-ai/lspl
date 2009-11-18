/*
 * Author: Antonov Vadim (avadim@gmail.com)
 */

#include <iostream>

#include "SynDictionary.h"
#include "Util.h"

namespace lspl {
	namespace dictionaries {

		void SynDictionary::LoadDictionary(const std::string &filename) {
			std::string text = Util::LoadTextFromFile(filename.c_str());
			std::vector<std::string> patterns;
			std::vector<std::vector<std::string> > synonim_patterns;
			Util::LoadSimilarPatterns(text, patterns, synonim_patterns);
			_patterns = Util::BuildPatterns(patterns);
			for(int i = 0; i < synonim_patterns.size(); ++i) {
				_synonim_patterns.push_back(Util::BuildPatterns(synonim_patterns[i]));
			}
		}

		bool SynDictionary::acceptsWords(const std::vector<std::string> &words)
				const {
			std::cout << "acceptWords";
			for(int i = 0; i < words.size(); ++i) {
				std::cout << Util::out.convert(words[i]) << " ";
			}
			std::cout << std::endl;

			if (words.size() < 2) {
				return true;
			}
			assert(words.size() == 2);
			for(int i = 0; i < _patterns->getPatternCount(); ++i) {
				patterns::PatternRef pattern = _patterns->getPatternByIndex(i);
				std::string normalized_match =
						Util::GetNormalizedMatch(words[0], pattern);
				if (normalized_match == "") {
					continue;
				}
				for(int j = 0; j < _synonim_patterns[j]->getPatternCount(); ++j) {
					patterns::PatternRef synonim_pattern =
							_synonim_patterns[j]->getPatternByIndex(j);
					std::string normalized_match =
							Util::GetNormalizedMatch(words[1], synonim_pattern);
					if (normalized_match == "") {
						continue;
					}
					return true;
				}
			}
			return false;
		}

		SynDictionary::SynDictionary(const std::string &name,
				const std::string &filename) : Dictionary(name) {
		}

		SynDictionary::~SynDictionary() {
		}

	} // namesoace dictionaries
} // namespace lspl

