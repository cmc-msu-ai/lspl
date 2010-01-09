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
			for(int i = 0; i < synonim_patterns.size(); ++i) {
				std::set<std::string> synonims_set;
				for(int j = 0; j < synonim_patterns[i].size(); ++j) {
					synonims_set.insert(synonim_patterns[i][j]);
				}
				_synonim_patterns.insert(
						std::pair<std::string, std::set<std::string> >(
								patterns[i], synonims_set));
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
			std::string word1 = Util::Normalize(words[0]);
			SynonimPatterns::const_iterator i = _synonim_patterns.find(word1);
			if (i != _synonim_patterns.end()) {
				std::string word2 = Util::Normalize(words[1]);
				std::set<std::string>::const_iterator j = i->second.find(word2);
				if (j != i->second.end()) {
					return true;
				}
			}
			return false;
		}

		bool SynDictionary::acceptsWords(const std::string &word1,
				const std::string &word2) const {
			std::vector<std::string> words;
			words.push_back(word1);
			words.push_back(word2);
			return acceptsWords(words);
		}

		SynDictionary::SynDictionary(const std::string &name,
				const std::string &filename) : Dictionary(name) {
			LoadDictionary(filename);
		}

		SynDictionary::~SynDictionary() {
		}

	} // namesoace dictionaries
} // namespace lspl

