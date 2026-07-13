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
#include <nlohmann/json.hpp>
#include "core/path.hpp"
#include "core/transaction.hpp"

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

void Budget::addTransaction(Transaction txn) {

}

void Budget::delTransaction() {

}

void Budget::save() {

	// Save metadata
	json metadata;	
	std::ifstream in(PATH / "metadata.json");

	if (!in.good()) {
		throw std::runtime_error("Budget Manager has not been initialized");
	}
	in >> metadata;
	in.close();

	json budget_json;

	budget_json["start_date"] = stringDate(*this->start_date);
	budget_json["end_date"] = stringDate(*this->end_date);
	budget_json["limit"] = this->limit;
	metadata["budgets"][this->name] = budget_json;

	std::ofstream out(PATH / "metadata.json");

	if (!out.is_open()) {
		throw std::runtime_error("Could not save metadata");
	}
	out << metadata.dump(4);
	out.close(); 

	// Save transactions
	std::ofstream csv(PATH / (this->name + ".csv"));

	if (!csv.is_open()) {
		throw std::runtime_error("Could not open file to save transactions");
	}

	// TODO: Placeholder header until transactions are implemented
	csv << "description,amount,date\n";

	csv.close();	
}

void Budget::load() {
	//loadMetaData;
	//loadTransactions;
}

std::chrono::system_clock::time_point Budget::parseDate(const std::string& s) {
	std::tm tm = {};

	std::istringstream ss(s);
	ss >> std::get_time(&tm, "%Y-%m-%d");

	// Bad format or extra characters
	if (ss.fail() || ss.peek() != EOF) {
		throw std::invalid_argument(
			"Invalid date format. Expected YYYY-MM-DD."
		);
	}

	// Save original values before mktime normalizes them
	int originalYear = tm.tm_year;
	int originalMonth = tm.tm_mon;
	int originalDay = tm.tm_mday;

	std::time_t t = std::mktime(&tm);

	if (t == -1) {
		throw std::invalid_argument("Invalid date.");
	}

	// Detect things like 2025-02-31 becoming 2025-03-03
	if (tm.tm_year != originalYear ||
		tm.tm_mon != originalMonth ||
		tm.tm_mday != originalDay) {
		throw std::invalid_argument("Invalid calendar date.");
	}

	return std::chrono::system_clock::from_time_t(t);
}

std::string Budget::stringDate(const std::chrono::system_clock::time_point& tp) {
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::localtime(&t);

    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d");

    return ss.str();
}
