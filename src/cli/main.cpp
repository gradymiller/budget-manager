// TODO: Save config options on machine, for default budget
#include <iostream>
#include <string>
#include "cli/budgetCmds.hpp"
#include "cli/transactionCmds.hpp"
#include "cli/categoryCmds.hpp"
#include "cli/otherCmds.hpp"
#include "cli/utilities.hpp"


int dispatch(int argc, char** argv) {

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
		return cmdSwitch(argv + 2);

	} else if (arg1 == "b" || arg1 == "budget") {
		if (arg2 == "add") {
			return budgetAdd(argv + 3);

		} else if (arg2 == "edit") {
			return budgetEdit(argv + 3);

		} else if (arg2 == "delete") {
			return budgetDelete(argv + 3);

		} else if (arg2 == "list") {
			return budgetList();

		} else if (arg2 == "-h" || arg2 == "--help") {
			return budgetHelp();

		} else {
			return cmdInvalid(arg2);
		}

	} else if (arg1 == "t" || arg1 == "transaction") {
		if (arg2 == "add") {
			return transactionAdd(argc - 3, argv + 3);

		} else if (arg2 == "edit") {
			return transactionEdit(argc - 3, argv + 3);

		} else if (arg2 == "delete") {
			return transactionDelete(argc - 3, argv + 3);

		} else if (arg2 == "list") {
			return transactionList(argc - 3, argv + 3);

		} else if (arg2 == "-h" || arg2 == "--help") {
			return transactionHelp();

		} else {
			return cmdInvalid(arg2);
		}

	} else if (arg1 == "c" || arg1 == "category") {
		// TODO: to be added in a later version
		if (arg2 == "add") {
			return categoryAdd(argc - 3, argv + 3);

		} else if (arg2 == "edit") {
			return categoryEdit(argc - 3, argv + 3);

		} else if (arg2 == "delete") {
			return categoryDelete(argc - 3, argv + 3);

		} else if (arg2 == "list") {
			return categoryList(argc - 3, argv + 3);

		} else if (arg2 == "-h" || arg2 == "--help") {
			return categoryHelp();

		} else {
			return cmdInvalid(arg2);
		}

	} else {
		return cmdInvalid(arg1);
	}

}

int main(int argc, char** argv) {
    try {
        return dispatch(argc, argv);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
