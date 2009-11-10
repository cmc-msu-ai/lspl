#include <iostream>

#include "DictionaryRecognizer.h"
#include "Util.h"

int main(int argc, char** argv) {
	if (argc < 3) {
		std::cout << "Usage: " << argv[0] << " patterns text " << std::endl;
		return 1;
	}
	lspl::DictionaryRecognizer *recognizer =
			new lspl::DictionaryRecognizer(argv[1], argv[2]);
	std::vector<lspl::PatternMatchRef> matches =
			recognizer->RecognizeAndSearch();
	int matches_count = 0;
	int patterns_not_null_count = 0;
	for(int i = 0; i < matches.size(); ++i) {
		std::cout << "'"
				//<< lspl::Util::out.convert(matches[i]->pattern->name)
				<< lspl::Util::out.convert(matches[i]->matches[0]->getFragment(0).getText())
				//<< matches[i]->matches[0]->getFragment(0).get
				<< "' " << matches[i]->match_count << " '"
				//<< " Normalized name: '"
				<< lspl::Util::out.convert(matches[i]->normalized_name)
				<< "'" << std::endl;
		matches_count += matches[i]->match_count;
		if (matches[i]->match_count) {
			++patterns_not_null_count;
		}
	}
	std::cout << "Total patterns: " << matches.size()
			<< " Patterns with matches: " << patterns_not_null_count
			<< " Total matches: " << matches_count << std::endl;
	delete recognizer;
	return 0;
}
