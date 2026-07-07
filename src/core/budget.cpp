// TODO: Add validation to setters
// TODO: Load data in from a csv (to struct/class), overwrite when saving

#include "budget.hpp"

#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <string>
#include <cctype>
#include <chrono>
#include <ctime>


const std::string& Budget::getName() const {
    return this->name;
}

std::chrono::system_clock::time_point Budget::getStartDate() const {
    return this->start_date;
}

std::chrono::system_clock::time_point Budget::getEndDate() const {
    return this->end_date;
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

void Budget::setStartDate(std::string sd) {
    this->start_date = parseDate(sd);
}

void Budget::setEndDate(std::string ed) {
    this->end_date = parseDate(ed);
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

std::string Budget::save() {
    // TODO: Save budget data to CSV/file
	return "TEST";
}

void Budget::load() {
    // TODO: Load budget data from CSV/file
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
