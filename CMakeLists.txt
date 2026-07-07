cmake_minimum_required(VERSION 3.28)

project(budget LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Get version from Git
execute_process(
    COMMAND git describe --tags --always --dirty
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)


# Generate version header
configure_file(
    ${CMAKE_SOURCE_DIR}/version.h.in
    ${CMAKE_BINARY_DIR}/generated/version.h
)


# External dependencies
include(FetchContent)

# JSON Header
FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_MakeAvailable(nlohmann_json)


# GoogleTest
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(googletest)


# Project library
add_library(budget_lib
    src/cli/budgetCmds.cpp
    src/cli/transactionCmds.cpp
    src/cli/categoryCmds.cpp
    src/cli/otherCmds.cpp
    src/cli/utilities.cpp
    src/core/init.cpp
    src/core/budget.cpp
)


target_include_directories(budget_lib
    PUBLIC
        src/core
        ${CMAKE_BINARY_DIR}/generated
)


target_compile_definitions(budget_lib
    PUBLIC
        APPVERSION="${GIT_VERSION}"
)


target_compile_options(budget_lib
    PRIVATE
        -Wall
        -Wextra
        -O2
)


# Link JSON dependency to library
# nlohmann_json is header-only, but this target provides the correct include paths
target_link_libraries(budget_lib
    PUBLIC
        nlohmann_json::nlohmann_json
)


# Main executable
add_executable(budget
    src/cli/main.cpp
)


target_link_libraries(budget
    PRIVATE
        budget_lib
)


# Testing
enable_testing()

add_executable(budget_test
    tests/cli_tests.cpp
)


target_link_libraries(budget_test
    PRIVATE
        budget_lib
        GTest::gtest_main
)


include(GoogleTest)

gtest_discover_tests(budget_test)
