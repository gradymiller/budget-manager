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


    // Complete budget loading
    Budget readBudget(int budgetId);


    // Budget persistence
    int createBudget(const Budget& budget);
    void updateBudget(const Budget& budget);
    void deleteBudget(int budget_id);


    // Category persistence
    int createCategory(const Category& category);
    void updateCategory(const Category& category);
    void deleteCategory(int category_id);


    // Transaction persistence
    int createTransaction(const Transaction& txn);
    void updateTransaction(const Transaction& txn);
    void deleteTransaction(int txn_id);

    std::vector<Transaction> readTransactions(
        const TransactionFilter& filter = {}
    );


private:
    void execSQL(const char* sql);

    sqlite3* db = nullptr;
};
