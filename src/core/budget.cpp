// TODO: change categories to dictionary instead of list
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

int Budget::getID() const {
	return this->id;
}

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
	double total = 0;

	for (const auto& [id, category] : categories) {
        total += category.getLimit();
    }

    return total;
}

std::unordered_map<int, Category> Budget::getCategories() const {
	return this->categories;
}

std::unordered_map<int, Transaction> Budget::getTransactions() const {
	return transactions;
}

Category Budget::getCategory(int category_id) {
	auto it = categories.find(category_id);

	if (it == categories.end()) {
		throw std::invalid_argument("Category not found");
	}

	return it->second;
}

Transaction Budget::getTransaction(int txn_id) {
	auto it = transactions.find(txn_id);

	if (it == transactions.end()) {
		throw std::invalid_argument("Transaction not found");
	}

	return it->second;
}

void Budget::setID(int id) {
	this->id = id;
}

// Budget names may contain only alphanumeric characters, underscores,
// and hyphens. Separators are not allowed at the beginning or end.
void Budget::setName(const std::string& n) {
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
	auto parsed_date = strToDate(sd);

	if (this->end_date && parsed_date > *this->end_date) {
		throw std::invalid_argument("Start Date cannot be after the End Date");
	} 

	this->start_date = parsed_date;
}

// Enforces a valid end date when a start date has been previously set.
void Budget::setEndDate(const std::string& ed) {
    auto parsed_date = strToDate(ed);

	if (this->start_date && parsed_date < *this->start_date) {
		throw std::invalid_argument("End Date cannot be before the Start Date");
	}

	this->end_date = parsed_date;
}

Category Budget::addCategory(const std::string& name,
						 const std::string& type,
						 const std::string& limit,
						 const std::string& preset) {

	if (name.empty() || type.empty() || limit.empty() || preset.empty()) {
		throw std::invalid_argument("New categories require a non-empty name, type, limit, and preset value");
	}

	Category category;
	category.setName(name);
	category.setType(type);
	category.setLimit(limit);
	category.setPreset(preset);

    this->categories[category.getID()] = category;
	return category;
}

int Budget::editCategory(const std::string& category_id,
						  const std::string& field,
						  const std::string& value) {

	int new_id = std::stoi(category_id);

	if (new_id == 1) {
		throw std::runtime_error("Cannot edit the unassigned category");
	}

	if (field == "name") {
		categories[new_id].setName(value);

	} else if (field == "type") {
		categories[new_id].setType(value);

	} else if (field == "limit") {
		categories[new_id].setLimit(value);
	
	} else if (field == "preset") {
		categories[new_id].setPreset(value);	

	} else {
		throw std::invalid_argument("Invalid field. Cannot edit");
	}

	return new_id;
}

int Budget::delCategory(const std::string& category_id) {
	int new_id = std::stoi(category_id);

	if (new_id == 1) {
		throw std::runtime_error("Cannot delete the unassigned category");
	}

	// Transactions must be updated to new categories before a category can be
	// deleted from the budget
	for (const auto& [id, txn] : transactions) {
		if (txn.getCategoryID() == new_id) {
			throw std::runtime_error("Cannot delete category that has transactions assigned to it. Change transaction categories first before removing the category");
		}
	}

	// Uses swap and pop_back to prevent O(n) deletion due to shifting.
	if (categories.erase(new_id) == 0) {
		throw std::invalid_argument("No category found");
	}
	return new_id;
}

Transaction Budget::addTransaction(const std::string& amount,
							const std::string& category_id,
							const std::string& type,
							const std::string& date,
							const std::string& vendor) {

	Transaction txn;
	txn.setAmount(amount);

	int new_id = std::stoi(category_id);
	txn.setCategoryID(new_id);

	txn.setType(type);


	// Optional input
	if (!date.empty()) {
		txn.setDate(date);
	}

	//Optional input
	if (!vendor.empty()) {
		txn.setVendor(vendor);
	}

	if (txn.getCategoryID() != 1) {
		if (txn.getType() == categories[new_id].getType()) {
			categories[new_id].addUsage(txn.getAmount()); 

		} else {
			categories[new_id].delUsage(txn.getAmount());
		}
	}

	transactions.emplace(next_id, std::move(txn));	

	// This is the only place that next_id is updated
	next_id++;

	return transactions[next_id - 1];
}

int Budget::editTransaction(const std::string& id,
							 const std::string& field,
							 const std::string& value) {

	int new_id = std::stoi(id);

	auto it = transactions.find(new_id);
	if (it == transactions.end()) {
		throw std::invalid_argument("Transaction not found");
	}

	double old_amt = transactions[new_id].getAmount();	

	if (field == "amount") {
		it->second.setAmount(value);

		double new_amt = it->second.getAmount();

		int c_index = it->second.getCategoryID();
		
		// Recalculate new category amount when transaction amount changes
		if (it->second.getType() == categories[c_index].getType()) {
			categories[c_index].delUsage(old_amt);
			categories[c_index].addUsage(new_amt);

		} else {
			categories[c_index].delUsage(new_amt);
			categories[c_index].addUsage(old_amt);
		}

	} else if (field == "category") {
		int c_id = transactions[new_id].getCategoryID();
		transactions[new_id].setCategoryID(std::stoi(value));
		int new_c_id = transactions[new_id].getCategoryID();
		
		if (c_id != 1) {
			if (categories[c_id].getType() == transactions[new_id].getType()) {
				categories[c_id].delUsage(transactions[new_id].getAmount());

			} else {
				categories[c_id].addUsage(transactions[new_id].getAmount());
			}
		}

		if (new_c_id != 1) {
			if (categories[new_c_id].getType() == transactions[new_id].getType()) {
				categories[new_c_id].addUsage(transactions[new_id].getAmount());

			} else {
				categories[new_c_id].delUsage(transactions[new_id].getAmount());
			}
		}

	} else if (field == "type") {
		transactions[new_id].setType(value);

	} else if (field == "date") {
		transactions[new_id].setDate(value);

	} else if (field == "vendor") {
		transactions[new_id].setVendor(value);

	} else {
		throw std::invalid_argument("Invalid field inputted");
	}

	return new_id;
}

int Budget::delTransaction(const std::string& id) {
    int new_id = std::stoi(id);

    auto it = transactions.find(new_id);
    if (it == transactions.end()) {
        throw std::invalid_argument("Transaction not found");
    }

    double amt = it->second.getAmount();
    int category_id = it->second.getCategoryID();

    auto cat_it = categories.find(category_id);
    if (cat_it == categories.end()) {
        throw std::runtime_error("Transaction references missing category");
    }

	if (cat_it->second.getID() != 1) {
		if (cat_it->second.getType() == it->second.getType()) {
			cat_it->second.delUsage(amt);

		} else {
			cat_it->second.addUsage(amt);
		}
	}

    transactions.erase(it);

    return new_id;
}

// TODO: Clean this up
void Budget::addCategory(const Category& category) {
	int id = category.getID();
	this->categories[id] = category;
}


// TODO: cleanup up usage of these duplicates
// TODO: could change cli access to create classes then use this version
// instead of full string version
void Budget::addTransaction(const Transaction& txn) {
	int id = txn.getID();
	this->transactions[id] = txn;
}


double Budget::getUsage() {
	return categories[1].getUsage();
}
