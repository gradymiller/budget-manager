#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "cli/cmdTemplate.hpp"
#include "core/budget.hpp"
#include "core/database.hpp"
#include "core/path.hpp"

namespace fs = std::filesystem;

// Create a new budget
int budgetAdd(int argc, const char* const argv[]) {

	// This is a template runner
    return runCommand([&]() {
		if (argc < 4) {
			throw std::invalid_argument("Too few arguments");
		}

		Database db(PATH / "budget-data.db");
		Budget budget = db.loadBudget();

        budget.setName(argv[0]);
        budget.setStartDate(argv[1]);
        budget.setEndDate(argv[2]);
        budget.setLimit(argv[3]);

		int budget_id = db.createBudget(budget);
		std::cout << "Budget ID: " << budget_id << '\n';
    });
}

int budgetEdit(int argc, const char* const argv[]) {

	// This is a template runner
    return runCommand([&]() {
		if (argc < 2) {
			throw std::invalid_argument("Too few arguments");
		}

		Database db(PATH / "budget-data.db");
		Budget budget = db.loadBudget();

        std::string field = argv[0];
        std::string value = argv[1];

        if (field == "name") {
			budget.setName(value);

        } else if (field == "start_date") {
            budget.setStartDate(value);

        } else if (field == "end_date") {
            budget.setEndDate(value);

        } else if (field == "limit") {
            budget.setLimit(value);

        } else {
            throw std::invalid_argument(field + " not known");
        }

		db.updateBudget(budget);
    });
}

int budgetDelete(int argc, const char* const argv[]) {

	// This is a template runner
    return runCommand([&]() {
		if (argc < 1) {
			throw std::invalid_argument("Too few arguments");
		}

		int budget_id = std::stoi(argv[0]);

		Database db(PATH / "budget-data.db");
		db.deleteBudget(budget_id);
    });
}

// TODO: Print out the budget metadata along with the names
int budgetList() {

	// This is a template runner
    return runCommand([&]() {
		std::cout << "NOTHING HERE RIGHT NOW" << '\n';
    });
}
