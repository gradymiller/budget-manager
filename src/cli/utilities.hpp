#pragma once

#include <string>
#include <chrono>
#include "core/transaction.hpp"

int cmdInvalid(std::string cmd);
std::string formatDate(std::chrono::system_clock::time_point date);
std::string typeToString(TransactionType type);
