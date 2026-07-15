// TODO: Implement all these
#include "category.hpp"
#include <stdexcept>
#include <sstream>

const std::string& Category::getName() const {
	return name;
}

CategoryType Category::getType() const {
	return type;
}

double Category::getLimit() const {
	return limit;
}

void Category::setName(const std::string& name) {
	for (size_t i=0; i < name.size(); i++) {
		char c = name[i];

		if (c == '_' || c == '-') {
			if (i == 0 || i == name.size() - 1) {
				std::ostringstream msg;
				msg << "'" << c << "' cannot be at the beginning or end of the category name";
				throw std::invalid_argument(msg.str());
			}

		} else if (!std::isalnum(static_cast<unsigned char>(c))) {
				std::ostringstream msg;
				msg << "'" << c << "' is not a valid character";
				throw std::invalid_argument(msg.str());
		}
	}
	this->name = name;
}

void Category::setType(const std::string& type) {
	if (type == "income") {
		this->type = CategoryType::Income;

	} else if (type == "expense") {
		this->type = CategoryType::Expense;

	} else {
		throw std::invalid_argument("Category type must be `income` or `expense`");
	}
}

void Category::setLimit(const std::string& limit) {
	double value;

	size_t pos;
	value = std::stod(limit, &pos);

	if (pos != limit.size()) {
		throw std::invalid_argument("Category limit must be a number");
	}

	if (value <= 0) {
		throw std::invalid_argument("Category limit must be greater than 0");
	}

	this->limit = value;
}
