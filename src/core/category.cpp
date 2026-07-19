// TODO: Implement all these
#include "category.hpp"

#include <iostream>
#include <stdexcept>
#include <sstream>

const std::string& Category::getName() const {
	return name;
}

const std::string& Category::getType() const {
	return type;
}

double Category::getLimit() const {
	return limit;
}

double Category::getUsage() const {
	return usage;
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

void Category::setType(std::string type) {
	std::transform(type.begin(), type.end(), type.begin(),
		[](unsigned char c) {
			return std::tolower(c);
	});

	if (type != "income" && type != "expense") {
		throw std::invalid_argument("Transaction type must be `income` or `expense`");

	} else {
		this->type = type;
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

void Category::setLimit(double limit) {
	if (limit <= 0) {
		throw std::invalid_argument("Category limit must be greater than 0");
	}

	this->limit = limit;
}

void Category::addUsage(double amt) {
	usage += amt;
}

void Category::delUsage(double amt) {
	double del = usage - amt;

	std::cout << usage << ", " << amt << '\n';
	if (del < 0) {
		throw std::invalid_argument("Usage cannot be less than 0");	
	}

	usage = del;
}
