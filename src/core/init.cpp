// TODO: Expand this with database, encryption, etc.
#include <filesystem>
#include <cstdlib>
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

void createConfig(fs::path dir) {
	if (!fs::exists(dir) || !fs::is_directory(dir)) {
		throw std::runtime_error("There is no valid directory to create aconfig file in\n");
	}

	fs::path file_path = dir / "config";
	std::ofstream(file_path, std::ios::app);

	if (!fs::exists(file_path)) {
		throw std::runtime_error("Failed to create config file\n");
	}	
}
