/**
 * Author: Vadim Antonov(avadim@gmail.com)
 *
 */

#include <boost/shared_ptr.hpp>
#include <cstdio>
#include <vector>
#include "RangeSetDecartTree.h"

#define NUMBER_OF_RANGES 100000

int main() {
	srand(10000);
	lspl::base::RangeSetDecartTree tree(false);
	std::vector<lspl::base::Range*> ranges;
	for(int i = 0; i < NUMBER_OF_RANGES; ++i) {
		if (i % 2 == 0) {
			ranges.push_back(new lspl::base::Range(NUMBER_OF_RANGES - i,
					NUMBER_OF_RANGES + i));
		} else {
			ranges.push_back(new lspl::base::Range(NUMBER_OF_RANGES + i * 2,
					NUMBER_OF_RANGES +  i * i));
		}
		tree.AddRange(*(ranges[i]));
	}
	if (tree.size() != NUMBER_OF_RANGES) {
		printf("Decart tree size %d isn't equal to %d\r\n", tree.size(),
				NUMBER_OF_RANGES);
	}
	for(int i = 0; i < NUMBER_OF_RANGES; ++i) {
		if (!tree.FindRange(*(ranges[i]))) {
			printf("Can't find range %d %d \r\n", ranges[i]->start, ranges[i]->end);
		}
		lspl::base::Range range(0, i);
		if (tree.FindRange(range)) {
			printf("We've found strange range in the tree %d %d \r\n",
					range.start, range.end);
		}
	}
	for(int i = 0; i < NUMBER_OF_RANGES; ++i) {
		if (!tree.FindRange(*(ranges[i]))) {
			printf("Can't find range %d %d \r\n", ranges[i]->start, ranges[i]->end);
		}
		tree.DeleteRange(*(ranges[i]));
	}
	if (tree.size() != 0) {
		printf("Some elemetns left in the tree.\r\n");
	}
	return 0;
}
