#pragma once

#include <optional>
#include <string>
#include <vector>

#include <sqlite3.h>

#include "core/budget.hpp"
#include "core/category.hpp"
#include "core/transaction.hpp"

struct TransactionFilter {
	std::optional<int> id;
	std::optional<int> categoryId;
	std::optional<int> budgetID;

	std::optional<std::string> vendor;

	std::optional<std::string> start_date;
	std::optional<std::string> end_date;
};

class Database {
public:
	Database(const std::string& filename);
	~Database();

	void createTables();

	void addBudget();
	void editBudget();
	void deleteBudget();
	void getBudgets();

	void addCategory();
	void editCategory();
	void deleteCategory();
	std::vector<Category> getCategories();

	void addTransaction();
	void editTransaction();
	void deleteTransaction();
	std::vector<Transaction> getTransactions(
		const TransactionFilter& filter = {}	
	);


private:
	sqlite3* db = nullptr;
};
