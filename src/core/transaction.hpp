#pragma once

#include <string>
#include <chrono>

enum class TransactionType { Expense, Income };

class Transaction {
public:
    Transaction() = default;

    double getAmount() const;
    const std::string& getCategory() const;
    TransactionType getType() const;
    std::chrono::system_clock::time_point getDate() const;
    const std::string& getVendor() const;

    void setAmount(const std::string& amount);
    void setCategory(const std::string& category);
    void setType(const std::string& type);
    void setDate(const std::string& date);
    void setVendor(const std::string& vendor);

private:
    double amount = 0.0;
    std::string category;
    TransactionType type = TransactionType::Expense;
    std::chrono::system_clock::time_point date{};
    std::string vendor;
};
