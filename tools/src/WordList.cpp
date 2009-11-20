/**
 * Author: Vadim Antonov(avadim@gmail.com)
 */

#include <assert.h>
#include <iostream>

#include "Util.h"
#include "WordList.h"

namespace lspl {
	WordList::WordList(std::string &text) : text(text), clear_text("") {
		GenerateWords();
		GenerateClearText();
	}

	void WordList::GenerateWords() {
		int i = 0, len = this->text.size();
		for (int j = 1; j < len; ++j) {
			if (Util::IsDelimiter(this->text[j])) {
				if (i < j) {
					words.push_back(this->text.substr(i, j - i));
					Util::ToUpper(words[words.size() - 1]);
				}
				i = j + 1;
			}
		}
		if (i < len - 1) {
			words.push_back(this->text.substr(i, len - i));
			Util::ToUpper(words[words.size() - 1]);
		}
	}

	void WordList::GenerateClearText() {
		int len = this->text.size();
		for (int i = 0; i < len; ++i) {
			if (!Util::IsDelimiter(this->text[i])) {
				this->clear_text += this->text[i];
			}
		}
		Util::ToUpper(this->clear_text);
	}

	int WordList::GetLength() const {
		return text.size();
	}

	int WordList::GetClearLength() const {
		return clear_text.size();
	}

	int WordList::Size() const {
		return words.size();
	}

	std::string WordList::GetWord(int index) const {
		assert(index >=0 && index < Size());
		return words[index];
	}

	std::string WordList::GetText() const {
		return text;
	}

	std::string WordList::GetClearText() const {
		return clear_text;
	}
} // namespace lspl
