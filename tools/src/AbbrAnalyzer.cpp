/**
 * Author: Vadim Antonov(avadim@gmail.com)
 */

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <vector>

#include "AbbrAnalyzer.h"
#include "Util.h"

namespace lspl {
	int AbbrAnalyzer::Analyze(std::string &text1, std::string &text2) {
		WordList list1(text1);
		WordList list2(text2);
		return Analyze(list1, list2);
	}

	int AbbrAnalyzer::Analyze(WordList &list1, WordList &list2) {
		if (list1.GetClearLength() == list2.GetClearLength() ||
				list1.GetClearText()[0] != list2.GetClearText()[0]) {
			return -1;
		}
		if (list1.GetClearLength() < list2.GetClearLength()) {
			return Analyze(list2, list1);
		}
		std::string text1 = list1.GetClearText();
		std::string text2 = list2.GetClearText();
		int common_len = FindCommonPart(text1, text2);
		if (common_len < list2.GetClearLength() ||
				((list1.Size() > 1) && !DeepAnalyze(list1, text2, 0, 0))) {
			return -1;
		}
		return 1;
	}

	bool AbbrAnalyzer::DeepAnalyze(WordList &list, std::string &text, int i, int j) {
		if (j == text.size()) {
			return false;
		}
		if (list.Size() - 1 == i) {
			std::string tmp_text = text.substr(j, text.size() - j);
			std::string word = list.GetWord(i);
			if (word[0] == tmp_text[0] &&
					FindCommonPart(word, tmp_text) == tmp_text.size()) {
				return true;
			} else {
				return false;
			}
		} else {
			assert(0 <= i && i < list.Size() - 1);
			std::string word = list.GetWord(i);
			// If current word is pretext we can skip it.
			if (Util::IsPretext(word) && DeepAnalyze(list, text, i + 1, j)) {
				return true;
			}
			if (word[0] != text[j]) {
				return false;
			}
			int len = text.size();
			std::string tmp_text = "";
			for (int k = j; (k < len - 1) && (k - j + 1 <= len); ++k) {
				tmp_text += text[k];
				if (FindCommonPart(word, tmp_text) == tmp_text.size() &&
						 DeepAnalyze(list, text, i + 1, k + 1)) {
					return true;
				}
			}
		}
		return false;
	}

	int AbbrAnalyzer::FindCommonPart(std::string &text1, std::string &text2) {
		int len1 = text1.size(), len2 = text2.size();
		std::vector< std::vector <int> > len(len1, std::vector<int>(len2, 0));
		for (int i = 0; i < len1; ++i) {
			for (int j = 0; j < len2; ++j) {
				if (i > 0) {
				 len[i][j] = len[i - 1][j];
				}
				if (j > 0) {
				 len[i][j] = std::max(len[i][j - 1], len[i][j]);
				}
				if (text1[i] == text2[j]) {
					if ((i > 0) && (j > 0)) {
						len[i][j] = std::max(len[i][j], len[i - 1][j - 1] + 1);
					} else {
					 len[i][j] = 1;
					}
				}
			}
		}
		return len[len1 - 1][len2 - 1];
	}

} // namespace lspl
