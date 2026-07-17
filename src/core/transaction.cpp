#include "transaction.hpp"

#include <string>
#include <stdexcept>
#include "core/utils.hpp"

double Transaction::getAmount() const {
    return amount;
}

const std::string& Transaction::getCategory() const {
    return category;
}

TransactionType Transaction::getType() const {
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

void Transaction::setCategory(const std::string& category) {
	this->category = category;
}

void Transaction::setType(const std::string& type) {
	if (type == "income") {
		this->type = TransactionType::Income;

	} else if (type == "expense") {
		this->type = TransactionType::Expense;

	} else {
		throw std::invalid_argument("Transaction type must be `income` or `expense`");
	}
}

void Transaction::setDate(const std::string& date) {
	auto parsed_date = parseDate(date);
	this->date = parsed_date;
}

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
