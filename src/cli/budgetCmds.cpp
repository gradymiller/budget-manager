// TODO: Add catches for saving/loading budgets
#include <iostream>
#include "budget.hpp"
#include "path.hpp"

int budgetAdd(int argc, char** argv) {
	try {
		// Init budget from core
		Budget budget;

		// Set basic params for the new budget
		budget.setName(argv[0]);
		budget.setStartDate(argv[1]);
		budget.setEndDate(argv[2]);
		budget.setLimit(argv[3]);
		
		// Save to csv file on user's local machine
		budget.save(PATH) << "\n";
		std::cout << "Budget '" << budget.getName() << "' saved\n";
		return 0;

	} catch (const std::invalid_argument& e) {
		std::cout << "Invalid Argument: " << e.what() << "\n";
		return 1;
	}
}

int budgetEdit(int argc, char** argv) {
	try {
		Budget budget;
		// TODO: Loading needs PATH and current budget
		budget.load();

		if (argv[0] == "name") {
			budget.setName(argv[1]);

		} else if (argv[0] == "start_date") {
			budget.setStartDate(argv[1]);

		} else if (arv[0] == "end_date") {
			budget.setEndDate(argv[1]);
		
		} else if (argv[0] == "limit") {
			budget.setLimit(argv[1]);

		} else {
			throw std::invalid_argument(argv[1]	+ " not known");
		}

		budget.save();

		return 0;
		
	} catch (const std::invalid_argument& e) {
		std::cout << "Invalid Argument: " << e.what() << "\n";
		return 1;
	} 
}

int budgetDelete(int argc, char** argv) {
	try {
		if (!fs::exists(PATH / argv[0])) {
			throw invalid_argument(argv[0] + ".csv does not exist");
		} else {
			std::filesystem::remove(PATH / argv[0]);
		}
		return 0;

	} catch (const std::invalid_argument& e) {
		std::cout << "Invalid Argument: " << e.what() << "\n";
		return 1;
	} 
}

int budgetList(int argc, char** argv) {
	for (const auto& entry : fs::directory_iterator(PATH)) {
		if (entry.is_regular_file() && entry.path().extension() == ".csv") {
			std::cout << entry.path().stem().string() << '\n';
		}
	}
	return 0;
}
