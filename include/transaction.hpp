#pragma once
#include <string>

enum class TransactionType {Expense, Income};

class Transaction {
public:
	Transaction(double amount, const std::string& category, const std::string& date,
				const std::string& vendor, TransactionType type);

	double getAmount() const;
	const std::string& getCategory() const;
	const std::string& getDate() const;
	const std::string& getVendor() const;
	TransactionType getType() const;

private:
	const double amount;
	const std::string category;
	const std::string date;
	const std::string vendor;
	const TransactionType type;
}
