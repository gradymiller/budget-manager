#pragma once
#include <string>
#include <chrono>

enum class TransactionType {Expense, Income};

class Transaction {
public:
	Transaction(double amount, const std::string& category, const std::chrono_system_clock::time_point& date,
				const std::string& vendor, TransactionType type);

	double getAmount() const;
	const std::string& getCategory() const;
	const std::chrono::system_clock::time_point& getDate() const;
	const std::string& getVendor() const;
	TransactionType getType() const;

private:
	const double amount;
	const std::string category;
	const std::chrono::system_clock::time_point date;
	const std::string vendor;
	const TransactionType type;
}
