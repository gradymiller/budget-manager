// TODO: Save config options on machine, for default budget
// TODO: Finish parsing everything here, keep it all to this file
#include <string>
#include "budgetCmds.hpp"
#include "transactionCmds.hpp"
#include "categoryCmds.hpp"
#include "otherCmds.hpp"
#include "utilities.hpp"


int main(int argc, char** argv) {

	int arg_idx = 0; // Current working argument

	// Check if a single arg was entered
	if (argc == 1) {
		//return printHelp();
		return 0;
	}

	arg_idx++; // Account for first arg
	std::string curr_arg = argv[arg_idx];

	if (curr_arg == "-h" || curr_arg == "--help") {
		//return printHelp();
		return 0;

	} else if (curr_arg == "-v" || curr_arg== "--version") {
		//return printVersion();
		return 0;

	} else if (curr_arg == "init") {
		//return cmdInit();
		return 0;

	} else if (curr_arg == "current") {
		//return cmdCurrent();
		return 0;

	} else if (argc == 2) {
		// TODO: Create utilities file for this and maybe the info.cpp
		//return cmdInvalid(curr_arg);
		return 0;

	} 

	// Parse commands that involve 3+ args
	if (curr_arg == "use" || curr_arg == "switch") {
		curr_arg = argv[arg_idx++];
		// TODO: Check if budget is valid
		// TODO: Set the new budget if possible
		// return cmdSwitch();
		return 0;

	} else if (curr_arg == "b" || curr_arg == "budget") {
		// TODO: parse the rest of the args
		arg_idx++;
		curr_arg = argv[arg_idx];

		if (curr_arg == "-h" || curr_arg == "--help") {
			//return budgetHelp();	
			return 0;

		} else if (curr_arg == "add") {
			return 0;

		} else if (curr_arg == "edit") {
			return 0;
			
		} else if (curr_arg == "delete") {
			return 0;
			
		} else if (curr_arg == "list") {
			return 0;
			
		} else {
			//return cmdInvalid(curr_arg);
			return 0;
		}

	} else if (curr_arg == "t" || curr_arg == "transaction") {
		// TODO: parse the rest of the args
		arg_idx++;
		curr_arg = argv[arg_idx];

		if (curr_arg == "-h" || curr_arg == "--help") {
			//return transactionHelp();	
			return 0;

		} else if (curr_arg == "add") {
			return 0;

		} else if (curr_arg == "edit") {
			return 0;
			
		} else if (curr_arg == "delete") {
			return 0;
			
		} else if (curr_arg == "list") {
			return 0;
			
		} else {
			//return cmdInvalid(curr_arg);
			return 0;
		}

	} else if (curr_arg == "c" || curr_arg == "category") {
		// TODO: to be added in a later version
		arg_idx++;
		curr_arg = argv[arg_idx];
		
		if (curr_arg == "-h" || curr_arg == "--help") {
			//return categoryHelp();	
			return 0;

		} else if (curr_arg == "add") {
			return 0;

		} else if (curr_arg == "edit") {
			return 0;
			
		} else if (curr_arg == "delete") {
			return 0;
			
		} else if (curr_arg == "list") {
			return 0;
			
		} else {
			//return cmdInvalid(curr_arg);
			return 0;
		}

	} else {
		// TODO: make it go to stderr instead of just stdout
		//return cmdInvalid(curr_arg);
		return 0;
	}

}
