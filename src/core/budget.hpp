#pragma once

#include <chrono>
#include <string>
#include <optional>
#include <vector>
#include "core/transaction.hpp"

class Budget {
public:
    Budget() = default;

    const std::string& getName() const;
    std::chrono::system_clock::time_point getStartDate() const;
    std::chrono::system_clock::time_point getEndDate() const;
    double getLimit() const;

    void setName(std::string n);
    void setStartDate(const std::string& sd);
    void setEndDate(const std::string& ed);
    void setLimit(std::string l);

	void addTransaction(Transaction txn);
	void delTransaction(); // TODO: takes in ID
	std::vector<Transaction> getTransactions();
	
	void addCategory(std::string category);
	void delCategory(std::string category);
	std::vectory<std::string> getCategories();

	void saveAll();
	void saveBudget();
	void saveTransactions();
	void saveCategories();

    void load();

private:
    std::string name;
	std::optional<std::chrono::system_clock::time_point> start_date;
	std::optional<std::chrono::system_clock::time_point> end_date;
    double limit = 0.0;
	std::vector<Transaction> transactions; 

	std::chrono::system_clock::time_point parseDate(const std::string& s);
	std::string stringDate(const std::chrono::system_clock::time_point& tp);
};
