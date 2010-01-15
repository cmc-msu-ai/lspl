#include <iostream>
#include <fstream>
#include <stdexcept>

#include <stdlib.h>

#include "tests/Tests.h"

#include "../cute/cute.h"
#include "../cute/ide_listener.h"
#include "../cute/cute_runner.h"

using namespace lspl;
using namespace lspl::tests;

int main() {
	cute::ide_listener lis;
	cute::makeRunner(lis)( refCountObjectsSuite(), "RefCount" );
	cute::makeRunner(lis)( patternStructureSuite(), "PatternStructure" );
	cute::makeRunner(lis)( patternBuildingSuite(), "PatternBuilding" );
	cute::makeRunner(lis)( dictionariesSuite(), "Dictionaries" );
	cute::makeRunner(lis)( matchingSuite(), "Matching" );

	return 0;
}
