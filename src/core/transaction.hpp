#pragma once

#include <string>
#include <chrono>

enum class TransactionType { Expense, Income };

class Transaction {
public:
	Transaction(
		double amount,
		const std::string& category,
		const std::chrono::system_clock::time_point& date,
		const std::string& vendor,
		TransactionType type
	);

	double getAmount() const;
	const std::string& getCategory() const;
	std::chrono::system_clock::time_point getDate() const;
	const std::string& getVendor() const;
	TransactionType getType() const;

private:
	double amount;
	std::string category;
	std::chrono::system_clock::time_point date;
	std::string vendor;
	TransactionType type;
};
