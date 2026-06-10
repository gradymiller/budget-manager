//TODO: Finish setter functions
#include "budget.hpp"
#include "transaction.hpp"

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

	void setName(std::string name) {}

	void setStartDate(std::string start_date) {}

	void setEndDate(std::string end_date) {}

	void setLimit(double limit) {}

private:
	std::string name;
	std::chrono::system_clock::time_point start_date;
	std::chrono::system_clock::time_point end_date;
	double limit;
}
