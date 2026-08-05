// TODO: Set budget_id and validate
#pragma once

#include <chrono>
#include <optional>
#include <string>
#include <unordered_map>

#include "core/category.hpp"
#include "core/transaction.hpp"

// Represents a complete budget, including its metadata, categories,
// transactions, and file persistence operations. The data is stored locally
// and is loaded into the class then saved back to local storage once finished.
class Budget {
public:
    // Constructs an empty budget where everything is accessed through setters
	// and getters.
    Budget() = default;

	// Returns the budget's id
	int getID() const;

    // Returns the budget's name.
    const std::string& getName() const;

    // Returns the budget's configured start date.
    std::chrono::system_clock::time_point getStartDate() const;

    // Returns the budget's configured end date.
    std::chrono::system_clock::time_point getEndDate() const;

    // Returns the overall spending limit. This is usually the combined total
	// of all your paychecks for a month.
    double getLimit() const;

    // Returns all budget categories. Specific category elements are accessed
	// through the respective category class.
    std::unordered_map<int, Category> getCategories() const;

    // Returns all recorded transactions indexed by transaction ID. These are
	// not in any specfic order even though the IDs increment as more are added
    std::unordered_map<int, Transaction> getTransactions() const;

	Category getCategory(int category_id);

	Transaction getTransaction(int txn_id);

	void setID(int id);

    // Updates the budget's name.
    void setName(const std::string& n);

    // Sets the budget's start date from a string representation. The start
	// date must be before the currently set end date, otherwise an error will
	// be thrown.
    void setStartDate(const std::string& sd);

    // Sets the budget's end date from a string representation. The end date
	// must be after the currently set start date, otherwise an error will be
	// thrown.
    void setEndDate(const std::string& ed);

    // Updates the overall spending limit for the specified date range. If this
	// is lowered after categories have been added, you may have to resize the
	// category limits first as they cannot exceed the overall limit.
    void setLimit(const std::string& l);

    // Adds a new category to the budget. Everything is passed in as strings
	// and is converted to an appropriate type.
    Category addCategory(const std::string& name,
                     const std::string& type,
                     const std::string& limit);

    // Modifies a field of an existing category.
    int editCategory(const std::string& category_id,
                      const std::string& field,
                      const std::string& value);

    // Removes a category from the budget. Transactions using the category that
	// is to be deleted must be updated before this operation is possible.
    int delCategory(const std::string& category_id);

    // Adds a transaction to the budget. Everything is read in as strings, then
	// is converted to an appropriate type internally. The date and vendor
	// arguments are optional, and can take in an empty string. The actual
	// optional feature is implemented at a higher level.
    Transaction addTransaction(const std::string& amount,
                        const std::string& category_id,
                        const std::string& type,
                        const std::string& date,
                        const std::string& vendor);

    // Modifies a field of an existing transaction.
	int editTransaction(const std::string& id,
                         const std::string& field,
                         const std::string& value);

    // Removes a transaction from the budget. Once the ID is deleted, the gap
	// will not be filled with another transaction, the IDs will continue
	// incrementing from the current spot.
    int delTransaction(const std::string& id);

	void addCategory(const Category&);
	void addTransaction(const Transaction&);
	double getUsage();

private:
    // Identifier assigned to the next transaction that is created. This should
	// always be one ahead of the highest transaction ID saved so far. This
	// value is saved to the json metadata file and is loaded in for previously
	// made budgets.
	int id;
    int next_id = 1;

    // Running totals used for reporting.
	// TODO: Implement these, only category totals are working as of now
    double spent = 0;
    double earned = 0;

    // Budget metadata.
    std::string name;
    std::optional<std::chrono::system_clock::time_point> start_date;
    std::optional<std::chrono::system_clock::time_point> end_date;

    // This is the total of all category limits for the budget. For a complete
	// budget, this value should equal the overall budget limit.
    double allocated = 0;

	// The overall limit for the current budget. This is mainly used to
	// restrict the category limits from exceeding the amount of money coming
	// in for the span of the budget.
    double limit = 0.0;

    // Categories defined for this budget. Invididual pieces of category
	// metadata are accessed through the Category class.
    std::unordered_map<int, Category> categories;

    // Transactions keyed by their unique identifier. Transactions should be
	// interacted with using the Transaction class.
    std::unordered_map<int, Transaction> transactions;
};

