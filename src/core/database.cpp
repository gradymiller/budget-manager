#include "database.hpp"

#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

#include "core/budget.hpp"
#include "core/category.hpp"
#include "core/transaction.hpp"
#include "core/utils.hpp"

#include "path.hpp"

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
		CREATE TABLE IF NOT EXISTS global_categories (
			id INTEGER PRIMARY KEY,
			name TEXT NOT NULL UNIQUE,
			type TEXT NOT NULL,
			default_limit REAL NOT NULL DEFAULT 0,
			preset INTEGER NOT NULL DEFAULT 1
		);
	)";

	const char* sql3 = R"(
		CREATE TABLE IF NOT EXISTS budget_categories (
			id INTEGER PRIMARY KEY AUTOINCREMENT,
			budget_id INTEGER NOT NULL,
			global_category_id INTEGER NOT NULL,
			name TEXT NOT NULL,
			type TEXT NOT NULL,
			category_limit REAL NOT NULL,
			usage REAL NOT NULL DEFAULT 0,

			FOREIGN KEY(budget_id)
				REFERENCES budgets(id),

			FOREIGN KEY(global_category_id)
				REFERENCES global_categories(id)
		);
	)";

	const char* sql4 = R"(
		CREATE TABLE IF NOT EXISTS transactions (
			id INTEGER PRIMARY KEY AUTOINCREMENT,
			amount REAL NOT NULL,
			category_id INTEGER NOT NULL,
			type TEXT NOT NULL,
			date TEXT,
			vendor TEXT,

			FOREIGN KEY(category_id)
				REFERENCES budget_categories(id)
		);
	)";

	const char* sql5 = R"(
		CREATE TABLE IF NOT EXISTS settings (
			key TEXT PRIMARY KEY,
			value TEXT NOT NULL
		);
	)";

	const char* sql6 = R"(
		CREATE TABLE IF NOT EXISTS vendors (
			id INTEGER PRIMARY KEY,
			name TEXT NOT NULL UNIQUE,
			global_category_id INTEGER NOT NULL,

			FOREIGN KEY(global_category_id)
				REFERENCES global_categories(id)
		);
	)";
	
	const char* sql7 = R"(
		INSERT OR IGNORE INTO global_categories
			(name, type, default_limit, preset)
		VALUES
			('unassigned', 'other', 0, 1);
	)";


	execSQL(sql1);
	execSQL(sql2);
	execSQL(sql3);
	execSQL(sql4);
	execSQL(sql5);
	execSQL(sql6);
	execSQL(sql7);
}

std::optional<std::string> Database::getSetting(const std::string& key)
{
    const char* sql = R"(
        SELECT value
        FROM settings
        WHERE key = ?;
    )";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    sqlite3_bind_text(
        stmt,
        1,
        key.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    std::optional<std::string> result;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* text = sqlite3_column_text(stmt, 0);
        result = std::string(reinterpret_cast<const char*>(text));
    }

    sqlite3_finalize(stmt);

    return result;
}

void Database::setSetting(const std::string& key,
                          const std::string& value)
{
    const char* sql = R"(
        INSERT INTO settings(key, value)
        VALUES(?, ?)
        ON CONFLICT(key)
        DO UPDATE SET value = excluded.value;
    )";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, value.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        throw std::runtime_error(error);
    }

    sqlite3_finalize(stmt);
}

Budget Database::loadBudget() {
    auto current = getSetting("current_budget");

    if (!current || current->empty()) {
        throw std::runtime_error("No current budget selected.");
    }

    int budget_id = std::stoi(*current);

    Budget budget;
    budget.setID(budget_id);

    sqlite3_stmt* stmt = nullptr;

    // Load budget metadata
    const char* budgetSQL = R"(
        SELECT name, start_date, end_date 
        FROM budgets
        WHERE id = ?;
    )";

    if (sqlite3_prepare_v2(db, budgetSQL, -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error(sqlite3_errmsg(db));

    sqlite3_bind_int(stmt, 1, budget_id);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Current budget not found.");
    }

    budget.setName(
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));

    budget.setStartDate(
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));

    budget.setEndDate(
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));

    sqlite3_finalize(stmt);

    // Load categories
    const char* categorySQL = R"(
        SELECT id,
               budget_id,
               name,
               type,
               category_limit,
               usage
        FROM budget_categories
        WHERE budget_id = ?;
    )";

    if (sqlite3_prepare_v2(db, categorySQL, -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error(sqlite3_errmsg(db));

    sqlite3_bind_int(stmt, 1, budget_id);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Category category;

        category.setID(sqlite3_column_int(stmt, 0));
        category.setBudgetID(sqlite3_column_int(stmt, 1));

        category.setName(
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));

        category.setType(
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));

        category.setLimit(sqlite3_column_double(stmt, 4));

        category.addUsage(sqlite3_column_double(stmt, 5));

        budget.addCategory(category);
    }

    sqlite3_finalize(stmt);

    // Load transactions
    const char* transactionSQL = R"(
        SELECT t.id,
               t.amount,
               t.category_id,
               t.type,
               t.date,
               t.vendor
        FROM transactions t
        JOIN budget_categories c
            ON c.id = t.category_id
        WHERE c.budget_id = ?;
    )";

    if (sqlite3_prepare_v2(db, transactionSQL, -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error(sqlite3_errmsg(db));

    sqlite3_bind_int(stmt, 1, budget_id);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Transaction txn;

        txn.setID(sqlite3_column_int(stmt, 0));

        txn.setAmount(
            std::to_string(sqlite3_column_double(stmt, 1)));

        txn.setCategoryID(sqlite3_column_int(stmt, 2));

        txn.setType(
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));

        if (sqlite3_column_type(stmt, 4) != SQLITE_NULL) {
            txn.setDate(
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        }

        if (sqlite3_column_type(stmt, 5) != SQLITE_NULL) {
            txn.setVendor(
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
        }

        budget.addTransaction(txn);
    }

    sqlite3_finalize(stmt);

    return budget;
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
			end_date = ?
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

	sqlite3_bind_int(
		stmt,
		4,
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
    const char* sql_transactions = R"(
        DELETE FROM transactions
        WHERE category_id IN (
            SELECT id
            FROM budget_categories
            WHERE budget_id = ?
        );
    )";

    const char* sql_categories = R"(
        DELETE FROM budget_categories
        WHERE budget_id = ?;
    )";

    const char* sql_budget = R"(
        DELETE FROM budgets
        WHERE id = ?;
    )";

    sqlite3_stmt* stmt = nullptr;

    // Delete transactions
    if (sqlite3_prepare_v2(
            db, sql_transactions, -1, &stmt, nullptr
        ) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    sqlite3_bind_int(stmt, 1, budget_id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        throw std::runtime_error(error);
    }

    sqlite3_finalize(stmt);

    // Delete categories
    if (sqlite3_prepare_v2(
            db, sql_categories, -1, &stmt, nullptr
        ) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    sqlite3_bind_int(stmt, 1, budget_id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        throw std::runtime_error(error);
    }

    sqlite3_finalize(stmt);

    // Delete budget
    if (sqlite3_prepare_v2(
            db, sql_budget, -1, &stmt, nullptr
        ) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    sqlite3_bind_int(stmt, 1, budget_id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        throw std::runtime_error(error);
    }

    sqlite3_finalize(stmt);
}

std::vector<Budget> Database::readBudgets() {
    const char* sql = R"(
        SELECT id,
               name,
			   start_date,
			   end_date
        FROM budgets 
    )";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    std::vector<Budget> budgets;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
		Budget budget;

        budget.setID(sqlite3_column_int(stmt, 0));
        budget.setName(
			reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        budget.setStartDate(
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        budget.setEndDate(
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));

        budgets.push_back(std::move(budget));
    }

    sqlite3_finalize(stmt);

    return budgets;
}

int Database::createCategory(const Category& category) {
    const char* globalSql = R"(
        INSERT INTO global_categories (
            name,
            type,
            preset,
            default_limit
        )
        VALUES (?, ?, ?, ?)
        ON CONFLICT(name) DO UPDATE SET
            type = excluded.type,
            preset = excluded.preset,
            default_limit = excluded.default_limit;
    )";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, globalSql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    sqlite3_bind_text(
        stmt, 1,
        category.getName().c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        stmt, 2,
        category.getType().c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_int(
        stmt, 3,
        category.getPreset() ? 1 : 0
    );

    sqlite3_bind_double(
        stmt, 4,
        category.getLimit()
    );

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        throw std::runtime_error(error);
    }

    sqlite3_finalize(stmt);

    // Get the global category ID.
    const char* idSql = R"(
        SELECT id
        FROM global_categories
        WHERE name = ?;
    )";

    if (sqlite3_prepare_v2(db, idSql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    sqlite3_bind_text(
        stmt, 1,
        category.getName().c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        throw std::runtime_error(error);
    }

    int globalCategoryID = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);

    // Create the budget-specific category.
    const char* categorySql = R"(
        INSERT INTO budget_categories (
            budget_id,
            global_category_id,
			name,
            type,
            category_limit,
            usage
        )
        VALUES (?, ?, ?, ?, ?, 0);
    )";

    if (sqlite3_prepare_v2(db, categorySql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    sqlite3_bind_int(stmt, 1, category.getBudgetID());
    sqlite3_bind_int(stmt, 2, globalCategoryID);

	sqlite3_bind_text(
		stmt,
		3,
		category.getName().c_str(),
		-1,
		SQLITE_TRANSIENT
	);

    sqlite3_bind_text(
        stmt,
		4,
        category.getType().c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_double(
        stmt,
		5,
        category.getLimit()
    );

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        throw std::runtime_error(error);
    }

    int categoryID =
        static_cast<int>(sqlite3_last_insert_rowid(db));

    sqlite3_finalize(stmt);

    return categoryID;
}

void Database::updateCategory(const Category& category) {
	const char* globalSql = R"(
		UPDATE global_categories
		SET
			type = ?,
			preset = ?,
			default_limit = ?
		WHERE id = (
			SELECT global_category_id
			FROM budget_categories
			WHERE id = ?
		);
	)";

	const char* categorySql = R"(
		UPDATE budget_categories
		SET
			type = ?,
			category_limit = ?,
			usage = ?
		WHERE id = ?;
	)";

	char* error = nullptr;

	if (sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, &error)
		!= SQLITE_OK) {
		std::string message = error;
		sqlite3_free(error);
		throw std::runtime_error(message);
	}

	sqlite3_stmt* stmt = nullptr;

	try {
		// Update global category
		if (sqlite3_prepare_v2(
				db, globalSql, -1, &stmt, nullptr) != SQLITE_OK) {
			throw std::runtime_error(sqlite3_errmsg(db));
		}

		sqlite3_bind_text(
			stmt, 1,
			category.getType().c_str(),
			-1,
			SQLITE_TRANSIENT
		);

		sqlite3_bind_int(
			stmt, 2,
			category.getPreset() ? 1 : 0
		);

		sqlite3_bind_double(
			stmt, 3,
			category.getLimit()
		);

		// budget_categories.id
		sqlite3_bind_int(
			stmt, 4,
			category.getID()
		);

		if (sqlite3_step(stmt) != SQLITE_DONE) {
			throw std::runtime_error(sqlite3_errmsg(db));
		}

		sqlite3_finalize(stmt);
		stmt = nullptr;

		// Update budget-specific category
		if (sqlite3_prepare_v2(
				db, categorySql, -1, &stmt, nullptr) != SQLITE_OK) {
			throw std::runtime_error(sqlite3_errmsg(db));
		}

		sqlite3_bind_text(
			stmt, 1,
			category.getType().c_str(),
			-1,
			SQLITE_TRANSIENT
		);

		sqlite3_bind_double(
			stmt, 2,
			category.getLimit()
		);

		sqlite3_bind_double(
			stmt, 3,
			category.getUsage()
		);

		sqlite3_bind_int(
			stmt, 4,
			category.getID()
		);

		if (sqlite3_step(stmt) != SQLITE_DONE) {
			throw std::runtime_error(sqlite3_errmsg(db));
		}

		sqlite3_finalize(stmt);
		stmt = nullptr;

		if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, &error)
			!= SQLITE_OK) {
			throw std::runtime_error(sqlite3_errmsg(db));
		}
	}
	catch (...) {
		if (stmt) {
			sqlite3_finalize(stmt);
		}

		sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
		throw;
	}
}

void Database::deleteCategory(int category_id) {
    const char* reassign_sql = R"(
        UPDATE transactions
        SET category_id = 1
        WHERE category_id = ?;
    )";

    const char* delete_sql = R"(
        DELETE FROM budget_categories
        WHERE id = ?;
    )";

    sqlite3_stmt* stmt = nullptr;

    // Reassign transactions to Uncategorized
    if (sqlite3_prepare_v2(
            db, reassign_sql, -1, &stmt, nullptr
        ) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    sqlite3_bind_int(stmt, 1, category_id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        throw std::runtime_error(error);
    }

    sqlite3_finalize(stmt);

    // Delete category
    if (sqlite3_prepare_v2(
            db, delete_sql, -1, &stmt, nullptr
        ) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    sqlite3_bind_int(stmt, 1, category_id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        throw std::runtime_error(error);
    }

    sqlite3_finalize(stmt);
}

std::vector<Category> Database::readCategories() {
    auto current = getSetting("current_budget");

    if (!current || current->empty()) {
        throw std::runtime_error("No current budget selected.");
    }

    int budget_id = std::stoi(*current);

    const char* sql = R"(
        SELECT id,
               budget_id,
               name,
               type,
               category_limit,
               usage
        FROM budget_categories
        WHERE budget_id = ?;
    )";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    sqlite3_bind_int(stmt, 1, budget_id);

    std::vector<Category> categories;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Category category;

        category.setID(sqlite3_column_int(stmt, 0));
        category.setBudgetID(sqlite3_column_int(stmt, 1));
        category.setName(
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        category.setType(
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        category.setLimit(sqlite3_column_double(stmt, 4));
        category.addUsage(sqlite3_column_double(stmt, 5));

        categories.push_back(std::move(category));
    }

    sqlite3_finalize(stmt);

    return categories;
}

std::vector<Category> Database::readPresets() {
	const char* sql = R"(
		SELECT name, type, preset, default_limit
		FROM global_categories
		WHERE preset = 1;
	)";

	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		throw std::runtime_error(sqlite3_errmsg(db));
	}

	std::vector<Category> categories;

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		Category category;

		category.setName(
			reinterpret_cast<const char*>(
				sqlite3_column_text(stmt, 0)
			)
		);

		category.setType(
			reinterpret_cast<const char*>(
				sqlite3_column_text(stmt, 1)
			)
		);

		category.setPreset(
			sqlite3_column_int(stmt, 2) ? "true" : "false"
		);

		category.setLimit(
			sqlite3_column_double(stmt, 3)
		);

		categories.push_back(category);
	}

	sqlite3_finalize(stmt);

	return categories;
}

int Database::createTransaction(const Transaction& txn, int budget_id) {
	// Start with the category that was given to the transaction.
	// This will normally be `unassigned` for automatically imported
	// transactions.
	int category_id = txn.getCategoryID();

	// If the transaction has a vendor, check whether we have
	// previously learned a category for that vendor.
	// Don't reassign vendor if the category has been manually assigned already
	if (txn.getVendor() && category_id == 1) {
		auto global_category_id =
			getVendorCategory(*txn.getVendor());

		// A vendor category was found.
		if (global_category_id) {
			// The vendor table stores a global category ID, but
			// transactions need the category ID belonging to the
			// current budget. Find that budget-specific category.
			const char* cSql = R"(
				SELECT id
				FROM budget_categories
				WHERE budget_id = ?
				AND global_category_id = ?;
			)";

			sqlite3_stmt* cStmt = nullptr;

			if (sqlite3_prepare_v2(
					db,
					cSql,
					-1,
					&cStmt,
					nullptr
				) != SQLITE_OK) {
				throw std::runtime_error(sqlite3_errmsg(db));
			}

			// Find the category with this global category ID
			// that belongs to the current budget.
			sqlite3_bind_int(
				cStmt,
				1,
				budget_id
			);

			sqlite3_bind_int(
				cStmt,
				2,
				*global_category_id
			);

			// If the category exists in this budget, use its
			// budget-specific ID for the transaction.
			if (sqlite3_step(cStmt) == SQLITE_ROW) {
				category_id = sqlite3_column_int(cStmt, 0);
			}

			sqlite3_finalize(cStmt);
		}
	}

	// Insert the transaction using the category we determined above.
	const char* sql = R"(
		INSERT INTO transactions
		(amount, category_id, type, date, vendor)
		VALUES (?, ?, ?, ?, ?);
	)";

	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(
			db,
			sql,
			-1,
			&stmt,
			nullptr
		) != SQLITE_OK) {
		throw std::runtime_error(sqlite3_errmsg(db));
	}

	sqlite3_bind_double(
		stmt,
		1,
		txn.getAmount()
	);

	// Use category_id rather than txn.getCategoryID(), since the
	// vendor lookup above may have changed the category.
	sqlite3_bind_int(
		stmt,
		2,
		category_id
	);

	sqlite3_bind_text(
		stmt,
		3,
		txn.getType().c_str(),
		-1,
		SQLITE_TRANSIENT
	);

	// Date is optional.
	if (txn.getDate()) {
		std::string date = dateToStr(*txn.getDate());

		sqlite3_bind_text(
			stmt,
			4,
			date.c_str(),
			-1,
			SQLITE_TRANSIENT
		);
	} else {
		sqlite3_bind_null(stmt, 4);
	}

	// Vendor is optional.
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
		std::string date = dateToStr(*txn.getDate());

		sqlite3_bind_text(
			stmt,
			4,
			date.c_str(),
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


	// If this transaction has a vendor, remember the category
	// that the user just assigned to it.
	if (txn.getVendor()) {
		const char* cSql = R"(
			SELECT global_category_id
			FROM budget_categories
			WHERE id = ?;
		)";

		sqlite3_stmt* cStmt = nullptr;

		if (sqlite3_prepare_v2(
				db,
				cSql,
				-1,
				&cStmt,
				nullptr
			) != SQLITE_OK) {
			throw std::runtime_error(sqlite3_errmsg(db));
		}

		sqlite3_bind_int(
			cStmt,
			1,
			txn.getCategoryID()
		);

		if (sqlite3_step(cStmt) == SQLITE_ROW) {
			int global_category_id =
				sqlite3_column_int(cStmt, 0);

			// Replace the vendor's previous category with
			// the category that was most recently assigned.
			setVendorCategory(
				*txn.getVendor(),
				global_category_id
			);
		}

		sqlite3_finalize(cStmt);
	}
}

void Database::deleteTransaction(int txn_id) {
	const char* sql = R"(
		DELETE FROM transactions
		WHERE id = ?;
	)";

	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		throw std::runtime_error(sqlite3_errmsg(db));
	}

	sqlite3_bind_int(stmt, 1, txn_id);

	if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        throw std::runtime_error(error);
    }

    sqlite3_finalize(stmt);
}

void Database::readTransactions() {
    const char* sql = R"(
        SELECT id, category_id, amount, type, date, vendor
        FROM transactions
		ORDER BY date DESC, id DESC;
    )";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    std::cout << "id, category_id, amount, type, date, vendor\n";

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* date =
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

        const char* vendor =
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));

        std::cout
            << sqlite3_column_int(stmt, 0) << ", "
            << sqlite3_column_int(stmt, 1) << ", "
            << sqlite3_column_double(stmt, 2) << ", "
            << sqlite3_column_text(stmt, 3) << ", "
            << (date ? date : "N/A") << ", "
            << (vendor ? vendor : "N/A") << '\n';
    }

    sqlite3_finalize(stmt);
}

void Database::setVendorCategory(const std::string& vendor, int global_category_id) {
	const char* sql = R"(
		INSERT INTO vendors (
			name,
			global_category_id
		)
		VALUES (?, ?)
		ON CONFLICT(name) DO UPDATE SET
			global_category_id = excluded.global_category_id;
	)";

	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		throw std::runtime_error(sqlite3_errmsg(db));
	}

	sqlite3_bind_text(
		stmt,
		1,
		vendor.c_str(),
		-1,
		SQLITE_TRANSIENT
	);

	sqlite3_bind_int(
		stmt,
		2,
		global_category_id
	);

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		std::string error = sqlite3_errmsg(db);
		sqlite3_finalize(stmt);
		throw std::runtime_error(error);
	}

	sqlite3_finalize(stmt);
}

std::optional<int> Database::getVendorCategory(const std::string& vendor) {
	const char* sql = R"(
		SELECT global_category_id
		FROM vendors
		WHERE name = ?;
	)";

	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		throw std::runtime_error(sqlite3_errmsg(db));
	}

	sqlite3_bind_text(
		stmt,
		1,
		vendor.c_str(),
		-1,
		SQLITE_TRANSIENT
	);

	if (sqlite3_step(stmt) == SQLITE_ROW) {
		int category_id = sqlite3_column_int(stmt, 0);
		sqlite3_finalize(stmt);
		return category_id;
	}

	sqlite3_finalize(stmt);
	return std::nullopt;
}
