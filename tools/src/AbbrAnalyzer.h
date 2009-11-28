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
		static int Analyze(const std::string &text1, const std::string &text2);
		static int Analyze(const WordList &list1, const WordList &list2);
	 private:
		// Solve problem about max common subsequence.
		static int FindCommonPart(const std::string &text1,
				const std::string &text2);
		// Analyze each word in the list.
		// i - index of the current analyzing word.
		// j - index in the text, from which we start to analyze the word.
		static bool DeepAnalyze(const WordList &list,
				const std::string &text, int i, int j);
	}; // AbbrAnalyzer
} // namespace lspl
#endif // __LSPL_ABBR_ANALYZER
