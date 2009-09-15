#include <iostream>
#include <fstream>
#include <stdexcept>

#include <stdlib.h>

#include "tests/Tests.h"

using namespace lspl;
using namespace lspl::tests;

int main() {

	testPatternStructure();
	testPatterns();
	testDictionaries();
	//testTextReaders();
	testMatching();

	std::cout << "Success!!!" << std::endl;
	std::cout << "Exiting..." << std::endl;

	return 0;
}
