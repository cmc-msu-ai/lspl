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
	recognizer->RecognizeAndSearch();
	//std::cout << lspl::Util::out.convert(lspl::Util::Normalize(lspl::Util::in.convert("чужих"))) << std::endl;
	delete recognizer;
	return 0;
}
