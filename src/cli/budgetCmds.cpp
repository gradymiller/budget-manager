// TODO: Add catches for saving/loading budgets
#include <iostream>
#include <string>
#include <filesystem>
#include "budget.hpp"
#include "path.hpp"

namespace fs = std::filesystem;

int budgetAdd(char** argv) {
	try {
		// Init budget from core
		Budget budget;

		// Set basic params for the new budget
		budget.setName(argv[0]);
		budget.setStartDate(argv[1]);
		budget.setEndDate(argv[2]);
		budget.setLimit(argv[3]);
		
		// Save to csv file on user's local machine
		std::cout << budget.save() << "\n";
		std::cout << "Budget '" << budget.getName() << "' saved\n";
		return 0;

	} catch (const std::invalid_argument& e) {
		std::cout << "Invalid Argument: " << e.what() << "\n";
		return 1;
	}
}

int budgetEdit(char** argv) {
	try {
		Budget budget;
		// TODO: Loading needs PATH and current budget
		budget.load();

		std::string arg0 = argv[0];

		if (arg0 == "name") {
			budget.setName(argv[1]);

		} else if (arg0 == "start_date") {
			budget.setStartDate(argv[1]);

		} else if (arg0 == "end_date") {
			budget.setEndDate(argv[1]);
		
		} else if (arg0 == "limit") {
			budget.setLimit(argv[1]);

		} else {
			throw std::invalid_argument(std::string(argv[1]) + " not known");
		}

		budget.save();

		return 0;
		
	} catch (const std::invalid_argument& e) {
		std::cout << "Invalid Argument: " << e.what() << "\n";
		return 1;
	} 
}

int budgetDelete(char** argv) {
	try {
		if (!fs::exists(PATH / argv[0])) {
			throw std::invalid_argument(std::string(argv[0]) + ".csv does not exist");
		} else {
			std::filesystem::remove(PATH / argv[0]);
		}
		return 0;

	} catch (const std::invalid_argument& e) {
		std::cout << "Invalid Argument: " << e.what() << "\n";
		return 1;
	} 
}

int budgetList() {
	for (const auto& entry : fs::directory_iterator(PATH)) {
		if (entry.is_regular_file() && entry.path().extension() == ".csv") {
			std::cout << entry.path().stem().string() << '\n';
		}
	}
	return 0;
}
