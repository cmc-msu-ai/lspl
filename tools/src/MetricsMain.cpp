#include <iostream>
#include <string>

#include "lspl/patterns/PatternBuilder.h"
#include "DictionaryRecognizer.h"
#include "SimilarityRecognizer.h"
#include "Util.h"

lspl::NamespaceRef LoadSimilarPatterns(const char *file) {
	lspl::NamespaceRef patterns_namespace = new lspl::Namespace();
	lspl::patterns::PatternBuilderRef builder =
			new lspl::patterns::PatternBuilder(patterns_namespace);
	std::string text = lspl::Util::LoadTextFromFile(file);
	return patterns_namespace;
}

int main(int argc, char** argv) {
	if (argc < 3) {
		std::cout << "Usage: " << argv[0] << " patterns text [similar patterns] "
				<< std::endl;
		return 1;
	}
	lspl::DictionaryRecognizer *recognizer =
			new lspl::DictionaryRecognizer(argv[1], argv[2]);
	//std::vector<lspl::PatternsMatch> terms = recognizer->RecognizeAndSearch();
	delete recognizer;
	char *similar_patterns_file = "similar_patterns.txt";
	if (argc == 4) {
		similar_patterns_file = argv[3];
	}
	lspl::NamespaceRef similar_pattenrs =
			LoadSimilarPatterns(similar_patterns_file);
	lspl::SimilarityRecognizer *similarity_recognizer =
			new lspl::SimilarityRecognizer(similar_patterns_file);
	/*recognizer = new lspl::DictionaryRecognizer(similar_patterns_file, argv[2]);
	std::vector<lspl::PatternsMatch> similar_terms =
			recognizer->RecognizeAndSearch();
	delete recognizer;*/
	return 0;
}
