/**
 * Author: Vadim Antonov(avadim@gmail.com)
 *
 * AbbrAnalyzer.
 */

#ifndef __LSPL_ABBR_ANALYZER
#define __LSPL_ABBR_ANALYZER

#include <string>
#include "WordList.h"

namespace lspl {
	class AbbrAnalyzer {
	 public:
		static int Analyze(std::string &text1, std::string &text2);
		static int Analyze(WordList &list1, WordList &list2);
	 private:
		// Solve problem about max common subsequence.
		static int FindCommonPart(std::string &text1, std::string &text2);
		// Analyze each word in the list.
		// i - index of the current analyzing word.
		// j - index in the text, from which we start to analyze the word.
		static bool DeepAnalyze(WordList &list, std::string &text, int i, int j);
	}; // AbbrAnalyzer
} // namespace lspl
#endif // __LSPL_ABBR_ANALYZER
