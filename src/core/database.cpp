#include "database.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

Database::Database(const std::string& filename) {
	if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK) {
		std::string error = sqlite3_errmsg(db);
		sqlite3_close(db);
		db = nullptr;

		throw std::runtime_error("Database Error: " + error);
	}	
}

Database::~Database() {
	if (db != nullptr) {
		sqlite3_close(db);
		db = nullptr;
	}
}
