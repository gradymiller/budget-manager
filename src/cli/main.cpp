// TODO: Save config options on machine, for default budget
#include <string>
#include "budgetCmds.hpp"
#include "transactionCmds.hpp"
#include "categoryCmds.hpp"
#include "otherCmds.hpp"
#include "utilities.hpp"


int main(int argc, char** argv) {

	// Check if a single arg was entered
	if (argc == 1) {
		return cmdHelp();
	}

	std::string arg1 = argv[1];

	// Dispatch cmds with no extra args
	if (arg1 == "-h" || arg1 == "--help") {
		return cmdHelp();

	} else if (arg1 == "-v" || arg1== "--version") {
		return cmdVersion();

	} else if (arg1 == "init") {
		return cmdInit();

	} else if (arg1 == "current") {
		return cmdCurrent();

	} 

	std::string arg2 = argv[2];

	//Dispatch commands with at least 1 extra arg
	if (arg1 == "use" || arg1 == "switch") {
		return cmdSwitch(argc - 2, argv + 2);

	} else if (arg1 == "b" || arg1 == "budget") {
		if (arg2 == "add") {
			budgetAdd(argc - 3, argv + 3);

		} else if (arg2 == "edit") {
			budgetEdit(argc - 3, argv + 3);

		} else if (arg2 == "delete") {
			budgetDelete(argc - 3, argv + 3);

		} else if (arg2 == "list") {
			budgetList(argc - 3, argv + 3);

		} else if (arg2 == "-h" || arg2 == "--help") {
			budgetHelp();

		} else {
			cmdInvalid(arg2);
		}

	} else if (arg1 == "t" || arg1 == "transaction") {
		if (arg2 == "add") {
			transactionAdd(argc - 3, argv + 3);

		} else if (arg2 == "edit") {
			transactionEdit(argc - 3, argv + 3);

		} else if (arg2 == "delete") {
			transactionDelete(argc - 3, argv + 3);

		} else if (arg2 == "list") {
			transactionList(argc - 3, argv + 3);

		} else if (arg2 == "-h" || arg2 == "--help") {
			transactionHelp();

		} else {
			cmdInvalid(arg2);
		}

	} else if (arg1 == "c" || arg1 == "category") {
		// TODO: to be added in a later version
		if (arg2 == "add") {
			categoryAdd(argc - 3, argv + 3);

		} else if (arg2 == "edit") {
			categoryEdit(argc - 3, argv + 3);

		} else if (arg2 == "delete") {
			categoryDelete(argc - 3, argv + 3);

		} else if (arg2 == "list") {
			categoryList(argc - 3, argv + 3);

		} else if (arg2 == "-h" || arg2 == "--help") {
			categoryHelp();

		} else {
			cmdInvalid(arg2);
		}

	} else {
		return cmdInvalid(arg1);
	}

}
