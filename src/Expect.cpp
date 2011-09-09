#include <iostream>
#include <stdlib.h>

#include "Expect.hpp"
#include "Test.hpp"

using namespace suboxTest;

void Expect::print(
		std::string const& file,
		int line,
		bool boolean,
		bool shouldAbort ){
	if( boolean ) {
		std::cout << "\033[0;32m[       OK ]\033[0m " << std::endl;
	} else {
		++runTest::expectFailed;
		std::cout << "\033[0;32m[\033[0m   \033[0;31mFAILED\033[0m\033[0;32m ]\033[0m at line: "
					<< line << " in file: " << file << std::endl;
		if( shouldAbort )
			throw std::exception();
	}
}

