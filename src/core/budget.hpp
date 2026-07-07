#pragma once

#include <chrono>
#include <string>

class Budget {
public:
    Budget() = default;

    const std::string& getName() const;
    std::chrono::system_clock::time_point getStartDate() const;
    std::chrono::system_clock::time_point getEndDate() const;
    double getLimit() const;

    void setName(std::string n);
    void setStartDate(std::string sd);
    void setEndDate(std::string ed);
    void setLimit(std::string l);

	std::string save();
    void load();

private:
    std::string name;
    std::chrono::system_clock::time_point start_date;
    std::chrono::system_clock::time_point end_date;
    double limit = 0.0;
	std::chrono::system_clock::time_point parseDate(const std::string& s);
};

