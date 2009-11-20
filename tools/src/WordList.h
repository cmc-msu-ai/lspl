/**
 * Author: Vadim Antonov(avadim@gmail.com)
 *
 * This class implements simple WordList.
 */

#ifndef __LSPL_ABBR_WORDLIST
#define __LSPL_ABBR_WORDLIST

#include <string>
#include <vector>

namespace lspl {
	class WordList {
	 public:
		WordList(std::string &text);
		// Length of the text.
		int GetLength() const;
		// Length of the clear_text.
		int GetClearLength() const;
		// Count of the words.
		int Size() const;
		std::string GetWord(int index) const;
		std::string GetText() const;
		std::string GetClearText() const;
	 private:
		// text contains default text.
		// clear_text contains default text wihtout delimiters.
		std::string text;
		std::string clear_text;
		std::vector <std::string> words;

		void GenerateClearText();
		void GenerateWords();
	};
} // namespace lspl

#endif //__LSPL_ABBR_WORDLIST
