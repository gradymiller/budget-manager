// TODO: Add catches for saving/loading budgets
#include <iostream>
#include <string>
#include <filesystem>
#include "budget.hpp"
#include "path.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
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
		budget.save();
		std::cout << "Budget '" << budget.getName() << "' saved\n";
		return 0;

	} catch (const std::invalid_argument& e) {
		std::cerr << "Invalid Argument: " << e.what() << "\n";
		return 1;
	}
}

int budgetEdit(char** argv) {
	try {
		Budget budget;
		// TODO: Loading needs PATH and current budget
		budget.load();

		std::string arg0 = argv[0];
		std::string arg1 = argv[1];

		if (arg0 == "name") {
			budget.setName(arg1);

		} else if (arg0 == "start_date") {
			budget.setStartDate(arg1);

		} else if (arg0 == "end_date") {
			budget.setEndDate(arg1);
		
		} else if (arg0 == "limit") {
			budget.setLimit(arg1);

		} else {
			throw std::invalid_argument(std::string(arg1) + " not known");
		}

		budget.save();

		return 0;
		
	} catch (const std::invalid_argument& e) {
		std::cerr << "Invalid Argument: " << e.what() << "\n";
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
		std::cerr << "Invalid Argument: " << e.what() << "\n";
		return 1;
	} 
}

int budgetList() {
    try {
        std::ifstream file(PATH / "metadata.json");
        if (!file) {
            throw std::runtime_error("Failed to open metadata.json");
        }

        json metadata;
        file >> metadata;

        if (!metadata.contains("budgets") || !metadata["budgets"].is_object()) {
            throw std::runtime_error("metadata.json does not contain a valid \"budgets\" object");
        }

        for (const auto& [name, _] : metadata["budgets"].items()) {
            std::cout << name << '\n';
        }

        return 0;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
