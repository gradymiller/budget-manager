#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

#include "core/budget.hpp"
#include "core/path.hpp"
#include "cli/cmdTemplate.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

int budgetAdd(const char* const argv[]) {
    return runCommand([&]() {
        Budget budget;

        budget.setName(argv[0]);
        budget.setStartDate(argv[1]);
        budget.setEndDate(argv[2]);
        budget.setLimit(argv[3]);

        budget.saveBudget();

        std::cout << "Budget '" << budget.getName() << "' saved\n";
    });
}

int budgetEdit(const char* const argv[]) {
    return runCommand([&]() {
        Budget budget;

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
    });
}

int budgetDelete(const char* const argv[]) {
    return runCommand([&]() {
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

int budgetList() {
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
