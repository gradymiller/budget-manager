#include <iostream>
#include <stdexcept>
#include "budget.hpp"


int categoryAdd(int argc, char** argv) {
	Budget budget;
	budget.load();

	if (argc < 1) {
		throw std::invalid_argument("Too few arguments");
	}

	budget.addCategory(argv[0]);	
	budget.save();
}

int categoryEdit(int argc, char** argv) {
	Budget budget;
	budget.load();
	return 0;
}

int categoryDelete(int argc, char** argv) {
	Budget budget;
	budget.load();

	if (argc < 1) {
		throw std::invalid_argument("Too few arguments");
	}

	budget.delCategory(argv[0]);	
	budget.save();
	return 0;
}

int categoryList(int argc, char** argv) {
	Budget budget;
	budget.load();

	budget.getCategories

	return 0;
}
