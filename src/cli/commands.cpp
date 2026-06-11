#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <stdexcept>
#include "init.hpp"

namespace fs = std::filesystem;

int cmdInit() {

	try {
		initBudgetManager();
		// TODO: createBudget(); ---> Make in CORE
		std::cout << "Budget Manager initialization complete\n";
		return 0;
		
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
		return 1;
	}
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

