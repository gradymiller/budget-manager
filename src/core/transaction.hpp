#pragma once

#include <string>
#include <chrono>
#include <optional>

class Transaction {
public:
    Transaction() = default;

    double getAmount() const;
    const std::string& getCategory() const;
	const std::string& getType() const;

	std::optional<std::chrono::system_clock::time_point> getDate() const;
    const std::optional<std::string>& getVendor() const;

    void setAmount(const std::string& amount);
    void setCategory(const std::string& category);
    void setType(std::string type);
    void setDate(const std::string& date);
    void setVendor(const std::string& vendor);

private:
    double amount = 0.0;
    std::string category;
	std::string type = "expense";

	std::optional<std::chrono::system_clock::time_point> date{};
	std::optional<std::string> vendor;
};
