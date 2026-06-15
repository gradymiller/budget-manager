#include <iostream>
#include <string>

int cmdInvalid(std::string cmd) {
	std::cerr << "Error: argument '" << cmd << "' not understood. Try passing --help instead";
	return 1;
}
