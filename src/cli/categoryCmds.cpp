#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include "core/budget.hpp"
#include "core/category.hpp"


int categoryAdd(int argc, const char* const argv[]) {
	try {
		Budget budget;
		budget.load();

		if (argc < 3) {
			throw std::invalid_argument("Too few arguments");
		}

		budget.addCategory(argv[0], argv[1], argv[2]);	

		budget.saveCategories();
		return 0;

	} catch (std::invalid_argument& e) {
		std::cerr << "Invalid Argument: " << e.what() << '\n';
		return 1;
	}
}

int categoryEdit(int argc, const char* const argv[]) {
	try {
		Budget budget;
		budget.load();

		if (argc < 3) {
			throw std::invalid_argument("Too few arguments");
		}

		budget.editCategory(argv[0], argv[1], argv[2]);

		budget.saveCategories();
		return 0;

	} catch (std::invalid_argument& e) {
		std::cerr << "Invalid Argument: " << e.what() << '\n';
		return 1;
	}
}

int categoryDelete(int argc, const char* const argv[]) {
	try {
		Budget budget;
		budget.load();

		if (argc < 1) {
			throw std::invalid_argument("Too few arguments");
		}

		budget.delCategory(argv[0]);	

		budget.saveCategories();
		return 0;

	} catch (std::invalid_argument& e) {
		std::cerr << "Invalid Argument: " << e.what() << '\n';
		return 1;
	}
}

int categoryList() {
	try {
		Budget budget;
		budget.load();

		std::vector<Category> categories = budget.getCategories();
		for (const auto& category : categories) {
			std::cout << category.getName() << ", Type: " << category.getType << ", Limit: " << "\n"; 	
		}

		return 0;

	} catch (std::invalid_argument& e) {
		std::cerr << "Invalid Argument: " << e.what() << '\n';
		return 1;
	}
}
