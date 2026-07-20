#pragma once

#include <string>

// Contains all the relevant information state for a budget category.
// It is saved as a sub-category in the same json file as the budgets are.
class Category {
public:
	Category() = default;

	// Returns the category's name.
	const std::string& getName() const;

	// Returns the category's type.
	const std::string& getType() const;

	// Returns the category's expected amount or limit.
	double getLimit() const;

	// Returns the amount used for the category.
	double getUsage() const;

	// Rename the category.
	void setName(const std::string& name);

	// Change the type. The options are `expense` or `income`.
	// An expense type represents a category where money goes out for the
	// duration of the budget while the income type represents cash flowing in
	// such as an expected check.
	void setType(std::string type);

	// The limit can be updated per category as long as it does not exceed the
	// budget's overall limit.
	void setLimit(const std::string& limit);

	// An alternative setter for the internal limit variable.
	// This allows the limit to be updated by inputting a number. This is used
	// in the budget class for simpler updating.
	void setLimit(double limit);

	// Add an amount to the current usage. This does not overwrite
	// the variable as the setters do.
	void addUsage(double amt);

	// Remove an amount from the current usage. This does not overwrite the
	// variable as the setters do.
	void delUsage(double amt);

private:
	std::string name;

	// This is not an enum class for the sake of converting to string output.
	std::string type = "expense";

	double limit = 0.0;
	double usage = 0.0;
};
