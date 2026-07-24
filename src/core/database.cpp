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


void Database::createTables() {
    // TODO: create SQL tables here
}


void Database::addBudget() {
    // TODO
}


void Database::editBudget() {
    // TODO
}


void Database::deleteBudget() {
    // TODO
}


void Database::getBudgets() {
    // TODO
}


void Database::addCategory() {
    // TODO
}


void Database::editCategory() {
    // TODO
}


void Database::deleteCategory() {
    // TODO
}


std::vector<Category> Database::getCategories() {
    // TODO: query categories
    return {};
}


void Database::addTransaction() {
    // TODO
}


void Database::editTransaction() {
    // TODO
}


void Database::deleteTransaction() {
    // TODO
}


std::vector<Transaction> Database::getTransactions(
	const TransactionFilter& filter) {
    // TODO: build SELECT query using filter
    return {};
}

