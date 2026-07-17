#include <iostream>
#include <vector>
#include <string>

#include "core/budget.hpp"
#include "core/category.hpp"
#include "cli/cmdTemplate.hpp"


int categoryAdd(int argc, const char* const argv[]) {
	return runCommand([&]() {
		Budget budget;
		budget.load();

		if (argc < 3) {
			throw std::invalid_argument("Too few arguments");
		}

		budget.addCategory(argv[0], argv[1], argv[2]);
		budget.saveCategories();
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
	});
}


int categoryList() {
	return runCommand([&]() {
		Budget budget;
		budget.load();

		std::vector<Category> categories = budget.getCategories();

		for (const auto& category : categories) {
			std::cout
				<< "Name: " << category.getName()
				<< ", Type: " << category.getType()
				<< ", Limit: " << category.getLimit()
				<< '\n';
		}
	});
}
