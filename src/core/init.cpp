// TODO: Expand this with database, encryption, etc.
// TODO: Use PATH so the default folder location can be configurable
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <stdexcept>

#include "database.hpp"
#include "path.hpp"

namespace fs = std::filesystem;

void setupFolder() {
	const char* home = std::getenv("HOME");

	if (!home) {
		throw std::runtime_error("Failed to find HOME envvar\n");
	}

	fs::create_directories(PATH);
}

void createFiles() {
    if (!fs::exists(PATH) || !fs::is_directory(PATH)) {
        throw std::runtime_error(
            "There is no valid directory to create config files in\n"
        );
    }

	Database db(PATH / "budget-data.db");
	db.createTables();
}
