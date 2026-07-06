#include "path.hpp"
#include <cstdlib>

std::string PATH = [] {
    const char* home = std::getenv("HOME");
    return std::string(home ? home : "") + "/.local/share/budget-manager/";
}();
