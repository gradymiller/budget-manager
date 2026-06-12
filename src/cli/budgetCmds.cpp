#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <stdexcept>
#include "init.hpp"
#include "budget.hpp"


int cmdInit() {

	try {
		//initBudgetManager();
		std::cout << "Budget Manager initialization complete\n";
		return 0;
		
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
		return 1;
	}
}

int cmdCreate(std::string name, std::string start_date, std::string end_date, double limit) {
	
	return 0;
}

int cmdAdd(int argc, char** argv, int curr_idx, std::string budget) {
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

