#pragma once

#include <chrono>
#include <string>
#include <optional>
#include <vector>
#include <unordered_map>
#include "core/transaction.hpp"
#include "core/category.hpp"

class Budget {
public:
    Budget() = default;

    const std::string& getName() const;
    std::chrono::system_clock::time_point getStartDate() const;
    std::chrono::system_clock::time_point getEndDate() const;
    double getLimit() const;
	std::vector<Category> getCategories();
	std::vector<Transaction> getTransactions();

    void setName(std::string n);
    void setStartDate(const std::string& sd);
    void setEndDate(const std::string& ed);
    void setLimit(std::string l);

	void addCategory(const std::string& name,
					 const std::string& type,
					 const std::string& limit);
	void editCategory(const std::string& category,
					  const std::string& field,
					  const std::string& value);
	void delCategory(const std::string& category);
	int findCategory(const std::string& category);

	void addTransaction(const std::string& amount,
						const std::string& category,
						const std::string& type,
						const std::string& date,
						const std::string& vendor);
	void editTransaction(const std::string& id,
						const std::string& field,
						const std::string& value);
	void delTransaction(const std::string& id);

	void saveAll();
	void saveBudget();
	void saveCategories();
	void saveTransactions();

    void load();

private:
    std::string name;
	std::optional<std::chrono::system_clock::time_point> start_date;
	std::optional<std::chrono::system_clock::time_point> end_date;
    double limit = 0.0;
	std::vector<Category> categories;
	std::unordered_map<Transaction> transactions; 
	int next_id;
};
