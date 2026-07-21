#pragma once

#include <chrono>
#include <string>

// Main convertor function to get strings validated to time_points
// Used in setters for anything with dates
std::chrono::system_clock::time_point strToDate(const std::string& s);

// Convertor for time_points to strings
// Mainly used for convenient output to stdout
std::string dateToStr(const std::chrono::system_clock::time_point& tp);

// Throws when an arg is invalid, used in main.cpp
int cmdInvalid(std::string cmd);
