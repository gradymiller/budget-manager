#pragma once

#include <string>

enum class CategoryType {
	Income,
	Expense
};

class Category {
public:
	Category() = default;

	const std::string& getName() const;
	CategoryType getType() const;
	double getLimit() const;

	void setName(const std::string& name);
	void setType(const std::string& type);
	void setLimit(const std::string& limit);

private:
	std::string name;
	CategoryType type = CategoryType::Expense;
	double limit = 0.0;
};
