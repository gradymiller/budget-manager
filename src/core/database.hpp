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
    std::optional<int> budgetId;

    std::optional<std::string> vendor;
    std::optional<std::string> startDate;
    std::optional<std::string> endDate;
};


class Database {
public:
    explicit Database(const std::string& filename);
    ~Database();

    void createTables();

	std::optional<std::string> getSetting(const std::string& key);
	void setSetting(const std::string& key, const std::string& value);

    // Complete budget loading
    Budget loadBudget();


    // Budget persistence
    int createBudget(const Budget& budget);
    void updateBudget(const Budget& budget);
    void deleteBudget(int budget_id);
	std::vector<Budget> readBudgets();


    // Category persistence
    int createCategory(const Category& category);
    void updateCategory(const Category& category);
    void deleteCategory(int category_id);
	std::vector<Category> readCategories();

	std::vector<Category> readPresets();


    // Transaction persistence
    int createTransaction(const Transaction& txn);
    void updateTransaction(const Transaction& txn);
    void deleteTransaction(int txn_id);

    void readTransactions();


private:
    void execSQL(const char* sql);

    sqlite3* db = nullptr;
};
