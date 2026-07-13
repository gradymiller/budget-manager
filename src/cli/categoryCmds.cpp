#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include "core/budget.hpp"


int categoryAdd(int argc, char** argv) {
	Budget budget;
	budget.load();

	if (argc < 1) {
		throw std::invalid_argument("Too few arguments");
	}

	budget.addCategory(argv[0]);	
	budget.saveCategories();
}

int categoryEdit(int argc, char** argv) {
	Budget budget;
	budget.load();

	if (argc < 2) {
		throw std::invalid_argument("Too few arguments");
	}

	budget.delCategory(argv[0]);
	budget.addCategory(argv[1]);

	return 0;
}

int categoryDelete(int argc, char** argv) {
	Budget budget;
	budget.load();

	if (argc < 1) {
		throw std::invalid_argument("Too few arguments");
	}

	budget.delCategory(argv[0]);	
	budget.saveCategories();
	return 0;
}

int categoryList(int argc, char** argv) {
	Budget budget;
	budget.load();

	std::vector<std::string> categories = budget.getCategories();
	for (std::string category : categories) {
		std::cout << category << "\n"; 	
	}

	return 0;
}
