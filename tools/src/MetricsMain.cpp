#include <iostream>
#include <string>

#include "lspl/patterns/PatternBuilder.h"
#include "DictionaryRecognizer.h"
#include "SimilarityRecognizer.h"
#include "Util.h"

void ExtractTerms(const std::vector<lspl::PatternMatchRef> &matches,
		std::vector<std::string> &terms) {
	terms.clear();
	std::cout << terms.size() << " " << matches.size() << std::endl;
	for(int i = 0; i < matches.size(); ++i) {
		terms.push_back(matches[i]->matches[0]->getFragment(0).getText());
	}
}

int main(int argc, char** argv) {
	if (argc < 4) {
		std::cout << "Usage: " << argv[0] <<
				" patterns1 patterns2 text [similar patterns] " << std::endl;
		return 1;
	}
	std::cout << "Patterns from file " << argv[1] << std::endl << std::endl;
	lspl::DictionaryRecognizer *recognizer =
			new lspl::DictionaryRecognizer(argv[1], argv[3]);
	std::vector<lspl::PatternMatchRef> matches1 =
			recognizer->RecognizeAndSearch();
	std::cout << std::endl << "Patterns from file " << argv[2] << std::endl <<
			std::endl;
	recognizer = new lspl::DictionaryRecognizer(argv[2], argv[3]);
	std::vector<lspl::PatternMatchRef> matches2 =
			recognizer->RecognizeAndSearch();
	delete recognizer;
	std::vector<std::string> terms1;
	ExtractTerms(matches1, terms1);
	std::vector<std::string> terms2;
	ExtractTerms(matches1, terms2);
	char *similar_patterns_file = "similarity_patterns.txt";
	if (argc == 5) {
		similar_patterns_file = argv[4];
	}
	std::cout << std::endl << "Similarity patterns from file " <<
			similar_patterns_file << std::endl << std::endl;
	lspl::SimilarityRecognizer *similarity_recognizer =
			new lspl::SimilarityRecognizer(similar_patterns_file);
	similarity_recognizer->FindSimilars(terms1, terms2);
	/*recognizer = new lspl::DictionaryRecognizer(similar_patterns_file, argv[2]);
	std::vector<lspl::PatternsMatch> similar_terms =
			recognizer->RecognizeAndSearch();
	delete recognizer;*/
	return 0;
}
