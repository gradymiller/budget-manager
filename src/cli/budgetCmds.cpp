// TODO: Add catches for saving/loading budgets
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

#include "core/budget.hpp"
#include "core/path.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

int budgetAdd(const char* const argv[]) {
    try {
        // Init budget from core
        Budget budget;

        // Set basic params for the new budget
        budget.setName(argv[0]);
        budget.setStartDate(argv[1]);
        budget.setEndDate(argv[2]);
        budget.setLimit(argv[3]);

        // Save to csv file on user's local machine
        budget.saveBudget();

        std::cout << "Budget '" << budget.getName() << "' saved\n";
        return 0;

    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid Argument: " << e.what() << '\n';
        return 1;
    }
}

int budgetEdit(const char* const argv[]) {
    try {
        Budget budget;

        // TODO: Loading needs PATH and current budget
        budget.load();

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

        budget.saveBudget();

        return 0;

    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid Argument: " << e.what() << '\n';
        return 1;
    }
}

int budgetDelete(const char* const argv[]) {
    try {
        fs::path budgetFile = PATH / (std::string(argv[0]) + ".csv");

        if (!fs::exists(budgetFile)) {
            throw std::invalid_argument(std::string(argv[0]) + ".csv does not exist");
        }

        fs::remove(budgetFile);

        return 0;

    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid Argument: " << e.what() << '\n';
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

    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
