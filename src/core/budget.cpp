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
    return this->limit;
}

std::unordered_map<int, Category> Budget::getCategories() const {
	return this->categories;
}

std::unordered_map<int, Transaction> Budget::getTransactions() const {
	return transactions;
}

Category Budget::getCategory(int category_id) {
	return categories[category_id];
}

Transaction Budget::getTransaction(int txn_id) {
	return transactions[txn_id];
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

void Budget::setLimit(const std::string& l) {
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

Category Budget::addCategory(const std::string& name,
						 const std::string& type,
						 const std::string& limit) {

	if (name.empty() || type.empty() || limit.empty()) {
		throw std::invalid_argument("New categories require a non-empty name, type and limit");
	}

	Category category;
	category.setName(name);
	category.setType(type);
	category.setLimit(limit);

	// Prevent category allocations from exceeding the overall budget limit.
	if ((allocated + category.getLimit()) > getLimit()) {
		throw std::invalid_argument("Category limit goes over the remaining amount of the budget limit");	
	}

    this->categories[category.getID()] = category;
	allocated += category.getLimit();
	
	return category;
}

int Budget::editCategory(const std::string& category_id,
						  const std::string& field,
						  const std::string& value) {

	int new_id = std::stoi(category_id);

	if (field == "name") {
		categories[new_id].setName(value);

	} else if (field == "type") {
		categories[new_id].setType(value);

	} else if (field == "limit") {
		double old_limit = categories[new_id].getLimit();

		categories[new_id].setLimit(value);
		double new_limit = categories[new_id].getLimit();
		
		// Verify the new limit does not exceed the overall budget limit before
		// updating.
		if ((allocated + (new_limit - old_limit)) > this->getLimit()) {
			categories[new_id].setLimit(old_limit);
			throw std::invalid_argument("New category limit exceeds remaining amount of the budget limit");		
		}

		allocated += categories[new_id].getLimit();


	} else {
		throw std::invalid_argument("Invalid field. Cannot edit");
	}

	return new_id;
}

int Budget::delCategory(const std::string& category_id) {
	int new_id = std::stoi(category_id);

	// Transactions must be updated to new categories before a category can be
	// deleted from the budget
	for (const auto& [id, txn] : transactions) {
		if (txn.getCategoryID() == new_id) {
			throw std::runtime_error("Cannot delete category that has transactions assigned to it. Change transaction categories first before removing the category");
		}
	}

	// Uses swap and pop_back to prevent O(n) deletion due to shifting.
	allocated -= categories[new_id].getLimit();
	categories.erase(new_id);

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

	categories[new_id].addUsage(txn.getAmount()); 

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
		transactions[new_id].setAmount(value);

		double new_amt = transactions[new_id].getAmount();

		int c_index = transactions[new_id].getCategoryID();
		
		// Recalculate new category amount when transaction amount changes
		categories[c_index].delUsage(old_amt);
		categories[c_index].addUsage(new_amt);

	} else if (field == "category") {
		transactions[new_id].setCategoryID(std::stoi(value));

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

	double amt = transactions[new_id].getAmount();

	auto removed = transactions.erase(new_id);
	
	if (removed == 0) {
		throw std::invalid_argument("Transaction not found");
	}
	
	categories[transactions[new_id].getCategoryID()].delUsage(amt);	
	
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
