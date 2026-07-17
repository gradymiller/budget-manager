// TODO: Add validation to setters
// TODO: Load data in from a csv (to struct/class), overwrite when saving

#include "core/budget.hpp"

#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <string>
#include <cctype>
#include <chrono>
#include <ctime>
#include <fstream>
#include <optional>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <nlohmann/json.hpp>
#include "core/path.hpp"
#include "core/transaction.hpp"
#include "core/category.hpp"
#include "core/utils.hpp"

using json = nlohmann::json;


const std::string& Budget::getName() const {
    return this->name;
}

std::chrono::system_clock::time_point Budget::getStartDate() const {
    return *this->start_date;
}

std::chrono::system_clock::time_point Budget::getEndDate() const {
    return *this->end_date;
}

double Budget::getLimit() const {
    return this->limit;
}

void Budget::setName(std::string n) {
	if (n.empty()) {
		throw std::invalid_argument("Name cannot be empty");
	}
	
	for (size_t i=0; i < n.size(); i++) {
		char c = n[i];

		if (c == '_' || c == '-') {
			if (i == 0 || i == n.size() - 1) {
				std::ostringstream msg;
				msg << "'" << c << "' cannot be at the beginning or end of the name";
				throw std::invalid_argument(msg.str());
			}

		} else if (!std::isalnum(static_cast<unsigned char>(c))) {
				std::ostringstream msg;
				msg << "'" << c << "' is not a valid character";
				throw std::invalid_argument(msg.str());
		}
	}

    this->name = std::move(n);
}

void Budget::setStartDate(const std::string& sd) {
	auto parsed_date = parseDate(sd);

	if (this->end_date && parsed_date > *this->end_date) {
		throw std::invalid_argument("Start Date cannot be after the End Date");
	} 

	this->start_date = parsed_date;
}

void Budget::setEndDate(const std::string& ed) {
    auto parsed_date = parseDate(ed);

	if (this->start_date && parsed_date < *this->start_date) {
		throw std::invalid_argument("End Date cannot be before the Start Date");
	}

	this->end_date = parsed_date;
}

void Budget::setLimit(std::string l) {
	double value;

	size_t pos;
	value = std::stod(l, &pos);

	if (pos != l.size()) {
		throw std::invalid_argument("Limit must be a number");	
	}

	if (value <= 0) {
		throw std::invalid_argument("Limit must be greater than 0");
	}

    this->limit = value;
}

void Budget::addCategory(const std::string& name, const std::string& type, const std::string& limit) {
	if (name.empty() || type.empty() || limit.empty()) {
		throw std::invalid_argument("New categories require a non-empty name, type and limit");
	}

	Category category;
	category.setName(name);
	category.setType(type);
	category.setLimit(limit);

    this->categories.push_back(category);
}

void Budget::editCategory(const std::string& category, const std::string& field, const std::string& value) {
	int index = findCategory(category);

	if (index == -1) {
		throw std::runtime_error("Category not previously saved");
	}

	if (field == "name") {
		categories[index].setName(value);

	} else if (field == "type") {
		categories[index].setType(value);

	} else if (field == "limit") {
		categories[index].setLimit(value);

	} else {
		throw std::invalid_argument("Invalid field. Cannot edit");
	}
}

void Budget::delCategory(const std::string& category) {
	int index = findCategory(category);

	if (index == -1) {
		throw std::runtime_error("Category not previously saved");
	}

	std::swap(categories[index], categories.back());
	this->categories.pop_back();
}

std::vector<Category> Budget::getCategories() {
	return this->categories;
}

int Budget::findCategory(const std::string& category) {
	for (size_t i = 0; i < categories.size(); i++) {
		if (categories[i].getName() == category) {
			return i;
		}	
	}
	return -1;
}

void Budget::addTransaction(const std::string& amount,
							const std::string& category,
							const std::string& type,
							const std::string& date,
							const std::string& vendor) {

	Transaction txn;
	txn.setAmount(amount);

	if (findCategory(category) == -1) {
		throw std::invalid_argument("Category not found");
	}

	txn.setCategory(category);
	txn.setType(type);
	txn.setDate(date);
	txn.setVendor(vendor);

	transactions.emplace(next_id, std::move(txn));				
	next_id++;
}

void Budget::editTransaction(const std::string& id,
							 const std::string& field,
							 const std::string& value) {

	int new_id = std::stoi(id);

	if (field == "amount") {
		transactions[new_id].setAmount(value);

	} else if (field == "category") {
		transactions[new_id].setCategory(value);

	} else if (field == "type") {
		transactions[new_id].setType(value);

	} else if (field == "date") {
		transactions[new_id].setDate(value);

	} else if (field == "vendor") {
		transactions[new_id].setVendor(value);

	} else {
		throw std::invalid_argument("Invalid field inputted");
	}
}

void Budget::delTransaction(const std::string& id) {
	int new_id = std::stoi(id);
	transactions.erase(new_id);
}

std::unordered_map<int, Transaction> Budget::getTransactions() {
	return transactions;
}

void Budget::saveBudget()
{
    json metadata;
    std::ifstream in(PATH / "metadata.json");

    if (!in.good()) {
        throw std::runtime_error("Budget Manager has not been initialized");
    }

    in >> metadata;
    in.close();

    // Update only the budget fields
    metadata["budgets"][this->name]["start_date"] = stringDate(*this->start_date);
    metadata["budgets"][this->name]["end_date"] = stringDate(*this->end_date);
    metadata["budgets"][this->name]["limit"] = this->limit;

    std::ofstream out(PATH / "metadata.json");

    if (!out.is_open()) {
        throw std::runtime_error("Could not save metadata");
    }

    out << metadata.dump(4);
    out.close();
}

void Budget::saveCategories() {
    // Read the current budget name
    std::ifstream current(PATH / "current");

    if (!current.is_open()) {
        throw std::runtime_error("No current budget selected");
    }

    std::string currentBudget;
    std::getline(current, currentBudget);
    current.close();


    // Load metadata.json
    json metadata;
    std::ifstream in(PATH / "metadata.json");

    if (!in.good()) {
        throw std::runtime_error("Budget Manager has not been initialized");
    }

    in >> metadata;
    in.close();


    // Convert categories to JSON
    json categories_json = json::array();

    for (const auto& category : categories) {
        categories_json.push_back({
            {"name", category.getName()},
            {"type", category.getType() == CategoryType::Expense ? "expense" : "income"},
            {"limit", category.getLimit()}
        });
    }


    // Save categories to the current budget
    metadata["budgets"][currentBudget]["categories"] = categories_json;


    // Write metadata back
    std::ofstream out(PATH / "metadata.json");

    if (!out.is_open()) {
        throw std::runtime_error("Could not save metadata");
    }

    out << metadata.dump(4);
    out.close();
}

void Budget::saveTransactions() {
	// Save transactions
	std::ofstream csv(PATH / (this->name + ".csv"));

	if (!csv.is_open()) {
		throw std::runtime_error("Could not open file to save transactions");
	}

	csv << "ID,Amount,Category,Type,Date,Vendor\n";

	for (const auto& [id, txn] : transactions) {
		csv << id << ","
			<< txn.getAmount() << ","
			<< txn.getCategory() << ","
			<< typeToString(txn.getType()) << ","
			<< (txn.getDate() ? formatDate(*txn.getDate()) : "") << ","
			<< txn.getVendor().value_or("") << '\n';
	}

	csv.close();	
}


void Budget::load() {
    std::ifstream current(PATH / "current");

    if (!current.is_open()) {
        throw std::runtime_error("No current budget selected");
    }

    std::getline(current, name);
    current.close();

    json metadata;

    std::ifstream in(PATH / "metadata.json");

    if (!in.is_open()) {
        throw std::runtime_error("Budget Manager has not been initialized");
    }

    in >> metadata;
    in.close();


    if (!metadata["budgets"].contains(name)) {
        throw std::invalid_argument("Budget does not exist");
    }


    json budget = metadata["budgets"][name];


    // Load budget metadata
    setStartDate(budget["start_date"].get<std::string>());
    setEndDate(budget["end_date"].get<std::string>());

    limit = budget["limit"].get<double>();


    // Load categories
    categories.clear();

    for (const auto& category_json : budget["categories"]) {
        Category category;

        category.setName(category_json["name"].get<std::string>());
        category.setType(category_json["type"].get<std::string>());

        category.setLimit(
            std::to_string(category_json["limit"].get<double>())
        );

        categories.push_back(category);
    }
}
