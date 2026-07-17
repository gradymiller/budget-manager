#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <stdexcept>


std::chrono::system_clock::time_point parseDate(const std::string& s) {
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

std::string stringDate(const std::chrono::system_clock::time_point& tp) {
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::localtime(&t);

    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d");

    return ss.str();
}
