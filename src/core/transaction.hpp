#pragma once

#include <chrono>
#include <optional>
#include <string>

// Validates and stores info about a single transaction. Everything can be
// modified using setters. Nothing is set by default. Higher-level functions
// can validate what must be set for a transaction to be valid.
class Transaction {
public:
    Transaction() = default;

	// Return the transaction value
    double getAmount() const;

	// Return the category, validated in budget class to be only currently
	// available categories.
    const std::string& getCategory() const;

	// Limited to `expense` and `income` stored as strings
	const std::string& getType() const;

	// Returns the date the transaction occurred
	// std::optional allows for the value to be empty
	std::optional<std::chrono::system_clock::time_point> getDate() const;

	// Returns the name of the vendor
	// std::optional allows for the value to be empty
    const std::optional<std::string>& getVendor() const;

	// Can be any valid decimal
    void setAmount(const std::string& amount);

	// Stores a string of the name of the category, not the category itself
    void setCategory(const std::string& category);

	// A string of `expense` or `income`
    void setType(std::string type);

	// Set as a timepoint for data analysis
    void setDate(const std::string& date);

	// Sets a string of the vendor name and has validation allowing certain
	// characters and formats
    void setVendor(const std::string& vendor);

private:
    double amount = 0.0;
    std::string category;
	std::string type = "expense";

	std::optional<std::chrono::system_clock::time_point> date{};
	std::optional<std::string> vendor;
};
