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
	std::string budget = "default";

	// Parse possible global options
	if (curr_arg == "-h" || curr_arg == "--help") {
		return printHelp();

	} else if (curr_arg == "-v" || curr_arg== "--version") {
		return printVersion();

	} else if (curr_arg == "-b" || curr_arg == "--budget") {
		if (argc == 2) {
			std::cout << "Invalid budget. Usage: budget --budget <name>";
			return 1;
		}
		budget = argv[2];
		arg_idx += 2; // Account for possible global arg
	}
	
	curr_arg = argv[arg_idx];

	// Parse cli commands, their options are not parsed here
	if (curr_arg == "init") {
		return cmdInit();

	} else if (curr_arg == "create") {
		return cmdCreate(argc, argv, arg_idx++, budget);

	} else if (curr_arg == "add") {
		return cmdAdd(argc, argv, arg_idx++, budget);

	} else if (curr_arg == "edit") {
		return cmdEdit(argc, argv, arg_idx++, budget);

	} else if (curr_arg == "delete") {
		return cmdDelete(argc, argv, arg_idx++, budget);

	} else if (curr_arg == "show") {
		return cmdShow(argc, argv, arg_idx++, budget);
		
	} else {
		std::cout << "Invalid command. Usage: budget [options] <command> [arguments]";
		return 1;
	}

}
