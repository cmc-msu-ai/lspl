/*
 * Author: Antonov Vadim (avadim@gmail.com)
 */

#include <cstdio>
#include <string>

#include "Util.h"

void TestLevinshtainDistance(std::string word1, std::string word2, int result) {
	int testResult = lspl::Util::LevinshteinDistance(word1, word2);
	if (testResult != result) {
		fprintf(stderr, "Bad Levinshtein distance for words '%s' and '%s': "
				"%d instead of %d\n", word1.c_str(), word2.c_str(), testResult,
				result);
	}
}

void TestLevinshtainDistance() {
	printf("TestLevinshteinDistance\n");
	TestLevinshtainDistance("hello", "hello", 0);
	TestLevinshtainDistance("hello", "hallo", 1);
	TestLevinshtainDistance("yat", "toy", 3);
}

void TestOrphologicalVariants(std::string term1, std::string term2,
		bool result) {
	bool testResult = lspl::Util::IsOrphologicalVariant(term1, term2);
	if (result != testResult) {
		fprintf(stderr, "Bad orphological variants call for terms '%s' and '%s'"
				": %d instead of %d\n", term1.c_str(), term2.c_str(),
				(int)testResult, (int)result);
	}
}

void TestOrphologicalVariants() {
	printf("TestOrphologicalVariants\n");
	TestOrphologicalVariants("the world is mine", "the world is mine", true);
	TestOrphologicalVariants("the world is mine", "the world is", false);
	TestOrphologicalVariants("the wold is mie", "the world is mine", true);
	TestOrphologicalVariants("th wold is min", "the world is mine", false);
	TestOrphologicalVariants("th wold is mine", "the world is mine", false);
}

int main() {
	TestLevinshtainDistance();
	TestOrphologicalVariants();
}
