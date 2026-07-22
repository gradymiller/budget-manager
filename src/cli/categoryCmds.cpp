#include <iostream>
#include <string>
#include <vector>

#include "cli/cmdTemplate.hpp"
#include "core/budget.hpp"
#include "core/category.hpp"

int categoryAdd(int argc, const char* const argv[]) {
	return runCommand([&]() {
		Budget budget;
		budget.load();

		if (argc < 3) {
			throw std::invalid_argument("Too few arguments");
		}

		budget.addCategory(argv[0], argv[1], argv[2]);
		budget.saveCategories();
		budget.saveBudget();
	});
}


int categoryEdit(int argc, const char* const argv[]) {
	return runCommand([&]() {
		Budget budget;
		budget.load();

		if (argc < 3) {
			throw std::invalid_argument("Too few arguments");
		}

		budget.editCategory(argv[0], argv[1], argv[2]);
		budget.saveCategories();
		budget.saveBudget();
	});
}


int categoryDelete(int argc, const char* const argv[]) {
	return runCommand([&]() {
		Budget budget;
		budget.load();

		if (argc < 1) {
			throw std::invalid_argument("Too few arguments");
		}

		budget.delCategory(argv[0]);
		budget.saveCategories();
		budget.saveBudget();
	});
}


int categoryList() {
	return runCommand([&]() {
		Budget budget;
		budget.load();

		std::vector<Category> categories = budget.getCategories();

		// Outputs a table-like format to stdout
		for (const auto& category : categories) {
			std::cout
				<< "Name: " << category.getName()
				<< ", Type: " << category.getType()
				<< ", Limit: " << category.getLimit()
				<< '\n';
		}
	});
}
