#pragma once

#include <string>
#include <chrono>

class Budget {
public:
	Budget(const std::string& name, const std::chrono::system_clock::time_point& start_date, const			  std::chrono::system_clock::time_point& end_date, double limit);

	const std::string& getName() const;
	const std::string& getStartDate() const;
	const std::string& getEndDate() const;
	double getLimit() const;

	void setName(std::string name);
	void setStartDate(std::chrono::system_clock::time_point start_date);
	void setEndDate(std::chrono::system_clock::time_point end_date);
	void setLimit(double limit);

private:
	std::string name;
	std::chrono::system_clock::time_point start_date;
	std::chrono::system_clock::time_point end_date;
	double limit;
}
