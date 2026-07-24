#pragma once

#include <string>

#include <sqlite3.h>

class Database {
public:
	Database(const std::string& filename);
	~Database();

private:
	sqlite3* db = nullptr;
};
