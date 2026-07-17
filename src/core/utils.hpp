#pragma once

#include <string>
#include <chrono>
#include "core/transaction.hpp"

std::chrono::system_clock::time_point parseDate(const std::string& s);
std::string stringDate(const std::chrono::system_clock::time_point& tp);
std::string formatDate(std::chrono::system_clock::time_point date);
std::string typeToString(TransactionType type);

int cmdInvalid(std::string cmd);
