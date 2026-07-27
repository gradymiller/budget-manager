#include "database.hpp"

#include <iostream>
#include <stdexcept>
#include <string>


Database::Database(const std::string& filename) {
    if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_close(db);
        db = nullptr;

        throw std::runtime_error("Database error: " + error);
    }
}

Database::~Database() {
    if (db != nullptr) {
        sqlite3_close(db);
        db = nullptr;
    }
}

void Database::execSQL(const char* sql) {
	char* err_msg = nullptr;

	if (sqlite3_exec(db, sql, nullptr, nullptr, &err_msg) != SQLITE_OK) {
		std::string error = err_msg;
		sqlite3_free(err_msg);
		throw std::runtime_error("SQL Error: " + error);
	}
}

void Database::createTables() {
	const char* sql1 = R"(
		CREATE TABLE IF NOT EXISTS budgets (
			id INTEGER PRIMARY KEY AUTOINCREMENT,
			name TEXT NOT NULL,
			start_date TEXT NOT NULL,
			end_date TEXT NOT NULL
		);
	)";

	const char* sql2 = R"(
		CREATE TABLE IF NOT EXISTS categories (
			id INTEGER PRIMARY KEY AUTOINCREMENT,
			budget_id INTEGER NOT NULL,
			name TEXT NOT NULL,
			type TEXT NOT NULL,
			limit REAL NOT NULL,

			FOREIGN KEY(budget_id)
				REFERENCES budgets(id)
		);
	)";

	const char* sql3 = R"(
		CREATE TABLE IF NOT EXISTS transactions (
			id INTEGER PRIMARY KEY AUTOINCREMENT,
			amount REAL NOT NULL,
			category_id INTEGER NOT NULL,
			type TEXT NOT NULL,
			date TEXT,
			vendor TEXT,

			FOREIGN KEY(category_id)
				REFERENCES categories(id)
		);
	)";
	
	execSQL(sql1);
	execSQL(sql2);
	execSQL(sql3);
}


void Database::createBudget() {
    // TODO
}


void Database::updateBudget() {
    // TODO
}


void Database::deleteBudget() {
    // TODO
}


void Database::readBudgets() {
    // TODO
}


void Database::createCategory() {
    // TODO
}


void Database::updateCategory() {
    // TODO
}


void Database::deleteCategory() {
    // TODO
}


std::vector<Category> Database::readCategories() {
    // TODO: query categories
    return {};
}


void Database::createTransaction() {
    // TODO
}


void Database::updateTransaction() {
    // TODO
}


void Database::deleteTransaction() {
    // TODO
}


std::vector<Transaction> Database::readTransactions(
	const TransactionFilter& filter) {
    // TODO: build SELECT query using filter
    return {};
}

