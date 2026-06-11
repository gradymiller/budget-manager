#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <stdexcept>

namespace fs = std::filesystem;

int cmdInit() {
	std::string budget;
	std::cout << "Budget name (default: budget): ";
	std::getline(std::cin, budget);
	
	if (budget.empty()) {
		budget = "budget";
	}

	const char* home = std::getenv("HOME");
	if (!home) {
		throw std::runtime_error("HOME not set");
	}

	// Equivalent to "mkdir -p"
	fs::path dir = fs::path(home) / ".local/share/budget-manager";

	fs::create_directories(dir);

	//Equivalent to "touch"
	fs::path file = dir / (budget + ".csv");
	std::ofstream out(file, std::ios::app);
	if (!out) {
		std::cerr << "Failed to create budget file\n";
		return 1;
	}

	std::cout << "File saved at: " << file << "\n";

	return 0;
}


int cmdAdd(int argc, char** argv, int curr_idx, std::string budget) {
	return 0;
}
int cmdCreate(int argc, char** argv, int curr_idx, std::string budget) {
	return 0;
}
int cmdEdit(int argc, char** argv, int curr_idx, std::string budget) {
	return 0;
}
int cmdDelete(int argc, char** argv, int curr_idx, std::string budget) {
	return 0;
}
int cmdShow(int argc, char** argv, int curr_idx, std::string budget) {
	return 0;
}

