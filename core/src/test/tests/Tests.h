/*
 * PatternsTest.h
 *
 *  Created on: Sep 27, 2008
 *      Author: alno
 */

#ifndef _LSPL_TESTS_PATTERNSTEST_H_
#define _LSPL_TESTS_PATTERNSTEST_H_

#include "../cute/cute.h"

namespace lspl { namespace tests {

cute::suite patternStructureSuite();
cute::suite patternBuildingSuite();
cute::suite dictionariesSuite();
cute::suite matchingSuite();
cute::suite textReadersSuite();

} }

#endif//_LSPL_TESTS_PATTERNSTEST_H_
