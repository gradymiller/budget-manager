#include "transaction.hpp"

double Transaction::getAmount() const
{
    return amount;
}

const std::string& Transaction::getCategory() const
{
    return category;
}

std::chrono::system_clock::time_point Transaction::getDate() const
{
    return date;
}

const std::string& Transaction::getVendor() const
{
    return vendor;
}

TransactionType Transaction::getType() const
{
    return type;
}

void Transaction::setAmount(const std::string& amount)
{
    // TODO: implement
}

void Transaction::setCategory(const std::string& category)
{
    // TODO: implement
}

void Transaction::setDate(const std::string& date)
{
    // TODO: implement
}

void Transaction::setType(const std::string& type)
{
    // TODO: implement
}

void Transaction::setVendor(const std::string& vendor)
{
    // TODO: implement
}
