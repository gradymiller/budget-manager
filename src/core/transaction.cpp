#include "transaction.hpp"

#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <string>

#include "core/utils.hpp"

double Transaction::getAmount() const {
    return amount;
}

int Transaction::getCategoryID() const {
    return category_id;
}

const std::string& Transaction::getType() const {
    return type;
}

std::optional<std::chrono::system_clock::time_point> Transaction::getDate() const {
    return date;
}

const std::optional<std::string>& Transaction::getVendor() const {
    return vendor;
}

void Transaction::setAmount(const std::string& amount) {
	double value;

	size_t pos;
	value = std::stod(amount, &pos);

	if (pos != amount.size()) {
		throw std::invalid_argument("Transaction amount must be a number");
	}

	if (value <= 0) {
		throw std::invalid_argument("Transaction amount must be greater than 0");
	}

	this->amount = value;
}

void Transaction::setCategoryID(int category_id) {
	this->category_id = category_id;
}

void Transaction::setType(std::string type) {
	
	// Convert to lowercase.
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

void Transaction::setDate(const std::string& date) {
	
	// Helper from utilities file
	auto parsed_date = strToDate(date);
	this->date = parsed_date;
}

// Contrains to alphanumeric characters. Hyphens and dashes are allowed as long
// as they are not at the beginning or end of the string.
void Transaction::setVendor(const std::string& vendor) {
	if (vendor.empty()) {
		throw std::invalid_argument("Vendor name cannot be empty");
	}
	
	for (size_t i=0; i < vendor.size(); i++) {
		char c = vendor[i];

		if (c == '_' || c == '-') {
			if (i == 0 || i == vendor.size() - 1) {
				std::ostringstream msg;
				msg << "'" << c << "' cannot be at the beginning or end of the vendor name";
				throw std::invalid_argument(msg.str());
			}

		} else if (!std::isalnum(static_cast<unsigned char>(c))) {
				std::ostringstream msg;
				msg << "'" << c << "' is not a valid character";
				throw std::invalid_argument(msg.str());
		}
	}

    this->vendor = std::move(vendor);
}
