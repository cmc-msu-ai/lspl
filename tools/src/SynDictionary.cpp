/*
 * Author: Antonov Vadim (avadim@gmail.com)
 */

#include <iostream>

#include "SynDictionary.h"
#include "Util.h"

namespace lspl {
	namespace dictionaries {

		void SynDictionary::LoadDictionary(const std::string &filename) {
		}

		bool SynDictionary::acceptsWords(const std::vector<std::string> &words)
				const {
			std::cout << "acceptWords";
			for(int i = 0; i < words.size(); ++i) {
				std::cout << Util::out.convert(words[i]) << " ";
			}
			std::cout << std::endl;
		}

		SynDictionary::SynDictionary(const std::string &name,
				const std::string &filename) : Dictionary(name) {
		}

		SynDictionary::~SynDictionary() {
		}

	} // namesoace dictionaries
} // namespace lspl

