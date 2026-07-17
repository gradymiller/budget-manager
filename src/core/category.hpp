#pragma once

#include <string>

class Category {
public:
	Category() = default;

	const std::string& getName() const;
	const std::string& getType() const;
	double getLimit() const;

	void setName(const std::string& name);
	void setType(std::string type);
	void setLimit(const std::string& limit);

private:
	std::string name;
	std::string type = "expense";
	double limit = 0.0;
};
