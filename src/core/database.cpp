#include "database.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

#include "core/budget.hpp"
#include "core/category.hpp"
#include "core/transaction.hpp"
#include "core/utils.hpp"


Database::Database(const std::string& filename) {
    if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_close(db);
        db = nullptr;

        throw std::runtime_error("Database error: " + error);
    }

	execSQL("PRAGMA foreign_keys = ON;");
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


int Database::createBudget(const Budget& budget) {
	const char* sql = R"(
		INSERT INTO budgets 
		(name, start_date, end_date)
		VALUES (?, ?, ?);
	)";

	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		throw std::runtime_error(sqlite3_errmsg(db));
	}

	sqlite3_bind_text(
		stmt,
		1,
		budget.getName().c_str(),
		-1,
		SQLITE_TRANSIENT
	);

	sqlite3_bind_text(
		stmt,
		2,
		dateToStr(budget.getStartDate()).c_str(),
		-1,
		SQLITE_TRANSIENT
	);

	sqlite3_bind_text(
		stmt,
		3,
		dateToStr(budget.getEndDate()).c_str(),
		-1,
		SQLITE_TRANSIENT
	);
	
	if (sqlite3_step(stmt) != SQLITE_DONE) {
		std::string error = sqlite3_errmsg(db);
		sqlite3_finalize(stmt);
		throw std::runtime_error(error);
	}

	sqlite3_finalize(stmt);

	return sqlite3_last_insert_rowid(db);
}


void Database::updateBudget(const Budget& budget) {
	const char* sql = R"(
		UPDATE budgets
		SET
			name = ?,
			start_date = ?,
			end_date = ?,
			limit = ?
		WHERE id = ?;
	)";

	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		throw std::runtime_error(sqlite3_errmsg(db));
	}

	sqlite3_bind_text(
		stmt,
		1,
		budget.getName().c_str(),
		-1,
		SQLITE_TRANSIENT
	);

	sqlite3_bind_text(
		stmt,
		2,
		dateToStr(budget.getStartDate()).c_str(),
		-1,
		SQLITE_TRANSIENT
	);

	sqlite3_bind_text(
		stmt,
		3,
		dateToStr(budget.getEndDate()).c_str(),
		-1,
		SQLITE_TRANSIENT
	);

	sqlite3_bind_double(
		stmt,
		4,
		budget.getLimit()
	);

	sqlite3_bind_int(
		stmt,
		5,
		budget.getID()
	);

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		std::string error = sqlite3_errmsg(db);
		sqlite3_finalize(stmt);
		throw std::runtime_error(error);
	}

	sqlite3_finalize(stmt);
}


void Database::deleteBudget(int budget_id) {
    // TODO
}


int Database::createCategory(const Category& category) {
	const char* sql = R"(
		INSERT INTO categories
		(budget_id, name, type, limit)
		VALUES (?, ?, ?, ?);
	)";

	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		throw std::runtime_error(sqlite3_errmsg(db));
	}

	sqlite3_bind_int(
		stmt,
		1,
		category.getBudgetID()
	);

	sqlite3_bind_text(
		stmt,
		2,
		category.getName().c_str(),
		-1,
		SQLITE_TRANSIENT
	);

	sqlite3_bind_text(
		stmt,
		3,
		category.getType().c_str(),
		-1,
		SQLITE_TRANSIENT
	);

	sqlite3_bind_double(
		stmt,
		4,
		category.getLimit()
	);

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		std::string error = sqlite3_errmsg(db);
		sqlite3_finalize(stmt);
		throw std::runtime_error(error);
	}

	sqlite3_finalize(stmt);

	return sqlite3_last_insert_rowid(db);
}


void Database::updateCategory(const Category& category) {
	const char* sql = R"(
		UPDATE categories
		SET
			budget_id = ?,
			name = ?,
			type = ?,
			limit = ?,
			usage = ?
		WHERE id = ?;
	)";

	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		throw std::runtime_error(sqlite3_errmsg(db));
	}

	sqlite3_bind_int(
		stmt,
		1,
		category.getBudgetID()
	);

	sqlite3_bind_text(
		stmt,
		2,
		category.getName().c_str(),
		-1,
		SQLITE_TRANSIENT
	);

	sqlite3_bind_text(
		stmt,
		3,
		category.getType().c_str(),
		-1,
		SQLITE_TRANSIENT
	);

	sqlite3_bind_double(
		stmt,
		4,
		category.getLimit()
	);

	sqlite3_bind_double(
		stmt,
		5,
		category.getUsage()
	);

	sqlite3_bind_int(
		stmt,
		6,
		category.getID()
	);

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		std::string error = sqlite3_errmsg(db);
		sqlite3_finalize(stmt);
		throw std::runtime_error(error);
	}

	sqlite3_finalize(stmt);
}


void Database::deleteCategory(int category_id) {
    // TODO
}


int Database::createTransaction(const Transaction& txn) {
	const char* sql = R"(
		INSERT INTO transactions
		(amount, category_id, type, date, vendor)
		VALUES (?, ?, ?, ?, ?);
	)";

	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		throw std::runtime_error(sqlite3_errmsg(db));
	}

	sqlite3_bind_double(
		stmt,
		1,
		txn.getAmount()
	);

	sqlite3_bind_int(
		stmt,
		2,
		txn.getCategoryID()
	);

	sqlite3_bind_text(
		stmt,
		3,
		txn.getType().c_str(),
		-1,
		SQLITE_TRANSIENT
	);

	if (txn.getDate()) {
		sqlite3_bind_text(
			stmt,
			4,
			dateToStr(*txn.getDate()).c_str(),
			-1,
			SQLITE_TRANSIENT
		);

	} else {
		sqlite3_bind_null(stmt, 4);
	}

	if (txn.getVendor()) {
		sqlite3_bind_text(
			stmt,
			5,
			txn.getVendor()->c_str(),
			-1,
			SQLITE_TRANSIENT
		);

	} else {
		sqlite3_bind_null(stmt, 5);
	}

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		std::string error = sqlite3_errmsg(db);
		sqlite3_finalize(stmt);
		throw std::runtime_error(error);
	}

	sqlite3_finalize(stmt);

	return sqlite3_last_insert_rowid(db);
}


void Database::updateTransaction(const Transaction& txn) {
	const char* sql = R"(
		UPDATE transactions 
		SET
			amount = ?,
			category_id = ?,
			type = ?,
			date = ?,
			vendor = ?
		WHERE id = ?;
	)";

	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		throw std::runtime_error(sqlite3_errmsg(db));
	}

	sqlite3_bind_double(
		stmt,
		1,
		txn.getAmount()
	);

	sqlite3_bind_int(
		stmt,
		2,
		txn.getCategoryID()
	);

	sqlite3_bind_text(
		stmt,
		3,
		txn.getType().c_str(),
		-1,
		SQLITE_TRANSIENT
	);

	if (txn.getDate()) {
		sqlite3_bind_text(
			stmt,
			4,
			dateToStr(*txn.getDate()).c_str(),
			-1,
			SQLITE_TRANSIENT
		);

	} else {
		sqlite3_bind_null(stmt, 4);
	}

	if (txn.getVendor()) {
		sqlite3_bind_text(
			stmt,
			5,
			txn.getVendor()->c_str(),
			-1,
			SQLITE_TRANSIENT
		);

	} else {
		sqlite3_bind_null(stmt, 5);
	}

	sqlite3_bind_int(
		stmt,
		6,
		txn.getID()
	);

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		std::string error = sqlite3_errmsg(db);
		sqlite3_finalize(stmt);
		throw std::runtime_error(error);
	}

	sqlite3_finalize(stmt);
}


void Database::deleteTransaction(int txn_id) {
    // TODO
}


std::vector<Transaction> Database::readTransactions(
	const TransactionFilter& filter) {
    // TODO: build SELECT query using filter
    return {};
}

