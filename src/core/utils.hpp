#pragma once

#include <string>
#include <chrono>

std::chrono::system_clock::time_point parseDate(const std::string& s);
std::string stringDate(const std::chrono::system_clock::time_point& tp);
std::string formatDate(std::chrono::system_clock::time_point date);

int cmdInvalid(std::string cmd);
