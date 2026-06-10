//TODO: Change time types to chrono ones
#pragma once
#include <string>
#include <chrono>

class Budget {
public:
	Budget(const std::string& name, const std::string& start_date, const std::string& end_date,
		   double limit);

	const std::string& getName() const;
	const std::string& getStartDate() const;
	const std::string& getEndDate() const;
	double getLimit() const;

	void setName(std::string name);
	void setStartDate(std::string start_date);
	void setEndDate(std::string end_date);
	void setLimit(double limit);

private:
	std::string name;
	std::string start_date;
	std::string end_date;
	double limit;
}
