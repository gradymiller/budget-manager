#include "core/budget.hpp"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include "core/category.hpp"
#include "core/path.hpp"
#include "core/transaction.hpp"
#include "core/utils.hpp"

using json = nlohmann::json;

const std::string& Budget::getName() const {
    return this->name;
}

// The budget guarantees the start date has been initialized before this
// accessor is called.
std::chrono::system_clock::time_point Budget::getStartDate() const {
    return *this->start_date;
}

// The budget guarantees the start date has been initialized before this
// accessor is called.
std::chrono::system_clock::time_point Budget::getEndDate() const {
    return *this->end_date;
}

double Budget::getLimit() const {
    return this->limit;
}

// Budget names may contain only alphanumeric characters, underscores,
// and hyphens. Separators are not allowed at the beginning or end.
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

// Enforces a valid start date when an end date has been previously set.
void Budget::setStartDate(const std::string& sd) {
	auto parsed_date = parseDate(sd);

	if (this->end_date && parsed_date > *this->end_date) {
		throw std::invalid_argument("Start Date cannot be after the End Date");
	} 

	this->start_date = parsed_date;
}

// Enforces a valid end date when a start date has been previously set.
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

void Budget::addCategory(const std::string& name,
						 const std::string& type,
						 const std::string& limit) {

	if (name.empty() || type.empty() || limit.empty()) {
		throw std::invalid_argument("New categories require a non-empty name, type and limit");
	}

	if (findCategory(name) != -1) {
		throw std::runtime_error("Cannot create categories with the same name");
	}

	Category category;
	category.setName(name);
	category.setType(type);
	category.setLimit(limit);

	// Prevent category allocations from exceeding the overall budget limit.
	if ((allocated + category.getLimit()) > getLimit()) {
		throw std::invalid_argument("Category limit goes over the remaining amount of the budget limit");	
	}

    this->categories.push_back(category);
	allocated += category.getLimit();
}

void Budget::editCategory(const std::string& category,
						  const std::string& field,
						  const std::string& value) {
	
	int index = findCategory(category);

	if (index == -1) {
		throw std::runtime_error("Category not previously saved");
	}

	if (field == "name") {
		categories[index].setName(value);

	} else if (field == "type") {
		categories[index].setType(value);

	} else if (field == "limit") {
		double old_limit = categories[index].getLimit();

		categories[index].setLimit(value);
		double new_limit = categories[index].getLimit();
		
		// Verify the new limit does not exceed the overall budget limit before
		// updating.
		if ((allocated + (new_limit - old_limit)) > this->getLimit()) {
			categories[index].setLimit(old_limit);
			throw std::invalid_argument("New category limit exceeds remaining amount of the budget limit");		
		}

		allocated += categories[index].getLimit();


	} else {
		throw std::invalid_argument("Invalid field. Cannot edit");
	}
}

void Budget::delCategory(const std::string& category) {
	int index = findCategory(category);

	if (index == -1) {
		throw std::runtime_error("Category not previously saved");
	}

	// Transactions must be updated to new categories before a category can be
	// deleted from the budget
	for (const auto& [id, txn] : transactions) {
		if (txn.getCategory() == category) {
			throw std::runtime_error("Cannot delete category that has transactions assigned to it. Change transaction categories first before removing the category");
		}
	}

	// Uses swap and pop_back to prevent O(n) deletion due to shifting.
	std::swap(categories[index], categories.back());
	allocated -= categories.back().getLimit();
	this->categories.pop_back();
}

std::vector<Category> Budget::getCategories() const {
	return this->categories;
}

// Primarily used within the budget class
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

	int c_index = findCategory(category);
	if (c_index == -1) {
		throw std::invalid_argument("Transaction category cannot be found");
	}

	txn.setCategory(category);
	txn.setType(type);

	// Optional input
	if (!date.empty()) {
		txn.setDate(date);
	}

	//Optional input
	if (!vendor.empty()) {
		txn.setVendor(vendor);
	}

	categories[c_index].addUsage(txn.getAmount()); 

	transactions.emplace(next_id, std::move(txn));	

	// This is the only place that next_id is updated
	next_id++;

}

void Budget::editTransaction(const std::string& id,
							 const std::string& field,
							 const std::string& value) {

	int new_id = std::stoi(id);

	auto it = transactions.find(new_id);
	if (it == transactions.end()) {
		throw std::invalid_argument("Transaction not found");
	}

	double old_amt = transactions[new_id].getAmount();	

	int c_index = findCategory(transactions[new_id].getCategory());
	if (c_index == -1) {
		throw std::runtime_error("Transaction category not found, could not delete");
	}

	if (field == "amount") {
		transactions[new_id].setAmount(value);

		double new_amt = transactions[new_id].getAmount();
		
		// Recalculate new category amount when transaction amount changes
		categories[c_index].delUsage(old_amt);
		categories[c_index].addUsage(new_amt);

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

	double amt = transactions[new_id].getAmount();

	int c_index = findCategory(transactions[new_id].getCategory());
	if (c_index == -1) {
		throw std::runtime_error("Transaction category not found, could not be deleted");
	}

	auto removed = transactions.erase(new_id);
	
	if (removed == 0) {
		throw std::invalid_argument("Transaction not found");
	}
	
	categories[c_index].delUsage(amt);	
}

std::unordered_map<int, Transaction> Budget::getTransactions() const {
	return transactions;
}

// Preserve the current budget's fields. This does not update categories
// to the json file even though the categories are nested within the
// budget as a field.
void Budget::saveBudget() {
	
	// Import the metadata.json file
    json metadata;
    std::ifstream in(PATH / "metadata.json");

    if (!in.good()) {
        throw std::runtime_error("Budget Manager has not been initialized");
    }

    in >> metadata;
    in.close();

	// Update the budget's fields here. When adding new fields to track, all
	// that needs to be done for saving is to add a new entry like the ones
	// below. The rest of the save function does not need to be touched.
    metadata["budgets"][this->name]["start_date"] = stringDate(*this->start_date);
    metadata["budgets"][this->name]["end_date"] = stringDate(*this->end_date);
    metadata["budgets"][this->name]["limit"] = this->limit;
	metadata["budgets"][this->name]["next_id"] = this->next_id;
	metadata["budgets"][this->name]["allocated"] = this->allocated;

	// Write back to the json file
    std::ofstream out(PATH / "metadata.json");

    if (!out.is_open()) {
        throw std::runtime_error("Could not save metadata");
    }

    out << metadata.dump(4);
    out.close();
}

// Only saves the state of the current budget's categories
void Budget::saveCategories() {
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
	// To add new category metadata just add a new entry to push_back()
    json categories_json = json::array();

    for (const auto& category : categories) {
        categories_json.push_back({
            {"name", category.getName()},
            {"type", category.getType()},
            {"limit", category.getLimit()},
			{"usage", category.getUsage()}
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

// Only saves the state of transactions for the current budget.
void Budget::saveTransactions() {
	// Save transactions
	std::ofstream csv(PATH / (this->name + ".csv"));

	if (!csv.is_open()) {
		throw std::runtime_error("Could not open file to save transactions");
	}

	// Write a header. This can be changed if new data is to be saved.
	csv << "ID,Amount,Category,Type,Date,Vendor\n";

	// Write each transaction to the csv file
	for (const auto& [id, txn] : transactions) {
		csv << id << ","
			<< txn.getAmount() << ","
			<< txn.getCategory() << ","
			<< txn.getType() << ","
			<< (txn.getDate() ? formatDate(*txn.getDate()) : "") << ","
			<< txn.getVendor().value_or("") << '\n';
	}

	csv.close();	
}


void Budget::load() {
	
	// Find and load the current budget
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


    // Load in the budget's metadata
    setStartDate(budget["start_date"].get<std::string>());
    setEndDate(budget["end_date"].get<std::string>());
    limit = budget["limit"].get<double>();
	next_id = budget["next_id"].get<int>();
	allocated = budget["allocated"].get<double>();


    // Load in the budget's categories and their fields
    categories.clear();

    for (const auto& category_json : budget["categories"]) {
        Category category;

        category.setName(category_json["name"].get<std::string>());
        category.setType(category_json["type"].get<std::string>());
        category.setLimit(std::to_string(category_json["limit"].get<double>()));
		category.addUsage(category_json["usage"].get<double>());

        categories.push_back(category);
    }

	// Load in the budget's transactions from the separate csv
	std::ifstream csv(PATH / (name + ".csv"));

	std::string line;
	std::getline(csv, line);

	while (std::getline(csv, line)) {
		std::stringstream ss(line);

		std::string id;
		std::string amount;
		std::string category;
		std::string type;
		std::string date;
		std::string vendor;

		std::getline(ss, id, ',');
		std::getline(ss, amount, ',');
		std::getline(ss, category, ',');
		std::getline(ss, type, ',');
		std::getline(ss, date, ',');
		std::getline(ss, vendor);

		Transaction txn;
		txn.setAmount(amount);
		txn.setCategory(category);
		txn.setType(type);

		if (!date.empty())
			txn.setDate(date);

		if (!vendor.empty())
			txn.setVendor(vendor);

		transactions.emplace(std::stoi(id), std::move(txn));
	}
}
