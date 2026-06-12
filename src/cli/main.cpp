// TODO: Save config options on machine, for default budget
// TODO: Finish parsing everything here, keep it all to this file
#include <iostream>
#include <string>
#include "commands.hpp"
#include "info.hpp"


int main(int argc, char** argv) {

	int arg_idx = 0; // Current working argument

	// Check if a single arg was entered
	if (argc == 1) {
		return printHelp();
	}

	arg_idx++; // Account for first arg
	std::string curr_arg = argv[arg_idx];

	if (curr_arg == "-h" || curr_arg == "--help") {
		return printHelp();

	} else if (curr_arg == "-v" || curr_arg== "--version") {
		return printVersion();

	} else if (curr_arg == "init") {
		return cmdInit();

	} else if (curr_arg == "current") {
		return cmdCurrent();

	} else if (curr_arg == "use" || curr_arg == "switch" || argc > 2) {
		curr_arg = argv[arg_idx++];
		// TODO: Check if budget is valid
		// TODO: Set the new budget if possible
		// return cmdSwitch();
		
	} else if (argc == 2) {
		// TODO: Create utilities file for this and maybe the info.cpp
		return cmdInvalid();
	}
	
	// Currently have the second arg selected and know there's at least 3 total
	// args

	if (curr_arg == "b" || curr_arg == "budget") {
		// TODO: parse the rest of the args
		return budgetAdd();

	} else if (curr_arg == "t" || curr_arg == "transaction") {
		// TODO: parse the rest of the args
		return cmdCreate();

	} else if (curr_arg == "c" || curr_arg == "category") {
		// TODO: to be added in a later version
		return 0;

	} else {
		// TODO: make it go to stderr instead of just stdout
		return cmdInvalid();
	}

}
