#include <iostream>
#include <string>

int cmdInvalid(std::string cmd) {
	std::cerr << "Error: argument '" << cmd << "' not understood. Try passing --help instead";
	return 1;
}

std::string formatDate(std::chrono::system_clock::time_point date) {
    auto time = std::chrono::system_clock::to_time_t(date);

    std::string result = std::ctime(&time);
    result.pop_back();

    return result;
}

std::string typeToString(TransactionType type) {
    switch (type)
    {
        case TransactionType::Expense:
            return "Expense";
        case TransactionType::Income:
            return "Income";
    }

    throw std::invalid_argument("Transaction type must be `income` or `expense`");
}
