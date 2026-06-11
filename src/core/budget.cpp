// TODO: Add validation to setters
// TODO: Load data in from a csv (to struct/class), overwrite when saving
#include <fstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include <chrono>

class Budget {
public:
	Budget(const std::string& name,
		   const std::chrono::system_clock::time_point& start_date,
		   const  std::chrono::system_clock::time_point& end_date,
		   double limit) : 
		   name(name), start_date(start_date), end_date(end_date), limit(limit) {}

	const std::string& getName() const {
		return name;
	}

	const std::string& getStartDate() const {
		return start_date;
	}

	const std::string& getEndDate() const {
		return end_date;
	}

	double getLimit() const {
		return limit;
	}

	void setName(std::string n) {
		this->name = n;
	}

	void setStartDate(std::string sd) {
		this->start_date = sd;
	}

	void setEndDate(std::string ed) {
		this->end_date = ed;
	}

	void setLimit(double l) {
		this->limit = l;
	}

	void save() {
		// write metadata to a file
		// write saved data to a file
	}

	std::pair<std::unordered_map, std::vector> load() {
		// load budget metadata in as a dictionary
		// load csv in as vector of struct/class
	}

private:
	std::string name;
	std::chrono::system_clock::time_point start_date;
	std::chrono::system_clock::time_point end_date;
	double limit;
}
