/*
 * Author: Antonov Vadim (avadim@gmail.com)
 *
 * Synonim dictionary.
 */

#ifndef __LSPL_SYN_DICTIONARY
#define __LSPL_SYN_DICTIONARY

#include <string>

#include <lspl/dictionaries/Dictionary.h>

namespace lspl {
	namespace dictionaries {

		class SynDictionary : public Dictionary {
		 private:
			void LoadDictionary(const std::string &filename);
		 protected:
			virtual bool acceptsWords(const std::vector<std::string> &words) const;
		 public:
			SynDictionary(const std::string &name, const std::string &filename);
			virtual ~SynDictionary();
		}; // SynDictionary

	} // namesoace dictionaries
} // namespace lspl

#endif // __LSPL_SYN_DICTIONARY
