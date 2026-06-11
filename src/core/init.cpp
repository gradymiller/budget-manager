#include <filesystem>
#include <cstdlib>
#include <stdexcept>

namespace fs = std::filesystem;

int initBudgetManager() {
	const char* home = std::getenv("HOME");

	if (!home) {
		throw std::runtime_error("Failed to find HOME envvar");
	}

	fs::path dir = fs::path(home) / ".local/share/budget-manager";
	fs::create_directories(dir);
	
	return 0;
}
