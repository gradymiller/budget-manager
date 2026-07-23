#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

#include "cli/cmdTemplate.hpp"
#include "core/budget.hpp"
#include "core/path.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

// Create a new budget
int budgetAdd(int argc, const char* const argv[]) {

	// This is a template runner
    return runCommand([&]() {
		if (argc < 4) {
			throw std::invalid_argument("Too few arguments");
		}

        Budget budget;

        budget.setName(argv[0]);
        budget.setStartDate(argv[1]);
        budget.setEndDate(argv[2]);
        budget.setLimit(argv[3]);

        budget.saveBudget();
        std::cout << "Budget '" << budget.getName() << "' saved\n";
    });
}

int budgetEdit(int argc, const char* const argv[]) {

	// This is a template runner
    return runCommand([&]() {
		if (argc < 2) {
			throw std::invalid_argument("Too few arguments");
		}
        Budget budget;

        budget.load();

        std::string field = argv[0];
        std::string value = argv[1];

        if (field == "name") {
			fs::path source = PATH / (budget.getName() + ".csv");

            budget.setName(value);

			// This is after the setName incase there is a validation error.
			fs::path destination = PATH / (value + ".csv");

			// Rename not necessary if transactions have not been saved before.
			// There will be no csv to rename.
			if (fs::exists(source)) {
				fs::rename(source, destination);
			}

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
    });
}

int budgetDelete(int argc, const char* const argv[]) {

	// This is a template runner
    return runCommand([&]() {
		if (argc < 1) {
			throw std::invalid_argument("Too few arguments");
		}
        std::string name = argv[0];

        fs::path budgetFile = PATH / (name + ".csv");
        fs::path metadataFile = PATH / "metadata.json";

        // Remove budget CSV if it exists
        if (fs::exists(budgetFile)) {
            fs::remove(budgetFile);
        }

        // Update metadata
        if (fs::exists(metadataFile)) {
            std::ifstream in(metadataFile);

            if (!in.is_open()) {
                throw std::runtime_error("Failed to open metadata.json");
            }

            json metadata;
            in >> metadata;
            in.close();

            if (metadata.contains("budgets") &&
                metadata["budgets"].is_object()) {

                metadata["budgets"].erase(name);
            }

            std::ofstream out(metadataFile);

            if (!out.is_open()) {
                throw std::runtime_error("Failed to save metadata.json");
            }

            out << metadata.dump(4);
            out.close();
        }
    });
}

// TODO: Print out the budget metadata along with the names
int budgetList() {

	// This is a template runner
    return runCommand([&]() {
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
    });
}
