// TODO: Add validation to setters
// TODO: Load data in from a csv (to struct/class), overwrite when saving

#include "budget.hpp"

#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <string>


const std::string& Budget::getName() const {
    return name;
}

std::chrono::system_clock::time_point Budget::getStartDate() const {
    return start_date;
}

std::chrono::system_clock::time_point Budget::getEndDate() const {
    return end_date;
}

double Budget::getLimit() const {
    return limit;
}

void Budget::setName(std::string n) {
    name = std::move(n);
}

void Budget::setStartDate(std::string sd) {
    start_date = parseDate(sd);
}

void Budget::setEndDate(std::string ed) {
    end_date = parseDate(ed);
}

void Budget::setLimit(std::string l) {
    limit = std::stod(l);
}

std::string Budget::save() {
    // TODO: Save budget data to CSV/file
	return "TEST";
}

void Budget::load() {
    // TODO: Load budget data from CSV/file
}

std::chrono::system_clock::time_point parseDate(const std::string& s) {
    std::tm tm = {};

    std::istringstream ss(s);
    ss >> std::get_time(&tm, "%Y-%m-%d");

    if (ss.fail()) {
        throw std::invalid_argument(
            "Invalid date format. Expected YYYY-MM-DD."
        );
    }

    std::time_t t = std::mktime(&tm);

    return std::chrono::system_clock::from_time_t(t);
}
