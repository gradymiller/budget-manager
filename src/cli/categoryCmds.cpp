#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include "core/budget.hpp"
#include "core/category.hpp"


int categoryAdd(int argc, const char* const argv[]) {
	Budget budget;
	budget.load();

	if (argc < 3) {
		throw std::invalid_argument("Too few arguments");
	}

	budget.addCategory(argv[0], argv[1], argv[2]);	

	budget.saveCategories();
	return 0;
}

int categoryEdit(int argc, const char* const argv[]) {
	Budget budget;
	budget.load();

	if (argc < 3) {
		throw std::invalid_argument("Too few arguments");
	}

	budget.editCategory(argv[0], argv[1], argv[2]);

	budget.saveCategories();
	return 0;
}

int categoryDelete(int argc, const char* const argv[]) {
	Budget budget;
	budget.load();

	if (argc < 1) {
		throw std::invalid_argument("Too few arguments");
	}

	budget.delCategory(argv[0]);	

	budget.saveCategories();
	return 0;
}

int categoryList() {
	Budget budget;
	budget.load();

	std::vector<Category> categories = budget.getCategories();
	for (const auto& category : categories) {
		std::cout << category.getName() << "\n"; 	
	}

	return 0;
}
