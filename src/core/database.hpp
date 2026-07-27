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

	void createBudget();
	void updateBudget();
	void deleteBudget();
	void readBudgets();

	void createCategory();
	void updateCategory();
	void deleteCategory();
	std::vector<Category> readCategories();

	void createTransaction();
	void updateTransaction();
	void deleteTransaction();
	std::vector<Transaction> readTransactions(
		const TransactionFilter& filter = {}	
	);


private:
	void execSQL(const char* sql);
	sqlite3* db = nullptr;
};
