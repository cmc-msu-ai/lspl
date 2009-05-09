#include "lspl/utils/Console.h"

#include <iostream>

#ifdef WIN32
const char * encoding = "CP866";
#else
const char * encoding = "UTF-8";
#endif

int main(int args, char ** argv) {
	lspl::utils::Console console( std::cin, std::cout, encoding );
	
	console.run();
	
	return 0;
}
