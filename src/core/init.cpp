// TODO: Expand this with database, encryption, etc.
// TODO: Use PATH so the default folder location can be configurable
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace fs = std::filesystem;

fs::path setupFolder() {
	const char* home = std::getenv("HOME");

	if (!home) {
		throw std::runtime_error("Failed to find HOME envvar\n");
	}

	fs::path dir = fs::path(home) / ".local/share/budget-manager";
	fs::create_directories(dir);
	
	return dir;
}

void createFiles(fs::path dir) {
    if (!fs::exists(dir) || !fs::is_directory(dir)) {
        throw std::runtime_error(
            "There is no valid directory to create config files in\n"
        );
    }

	Database db("budget-data.db");
	db.createTables();
}
