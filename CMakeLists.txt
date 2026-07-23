cmake_minimum_required(VERSION 3.28)

project(budget LANGUAGES CXX)

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


# Dependencies
include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_MakeAvailable(nlohmann_json)


FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(googletest)


# Core library
add_library(budget_lib
    src/cli/budgetCmds.cpp
    src/cli/transactionCmds.cpp
    src/cli/categoryCmds.cpp
    src/cli/otherCmds.cpp
    src/core/utils.cpp
    src/core/init.cpp
    src/core/budget.cpp
    src/core/transaction.cpp
    src/core/category.cpp
)


target_include_directories(budget_lib
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
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


target_link_libraries(budget_lib
    PUBLIC
        nlohmann_json::nlohmann_json
)


# Application
add_executable(bmcli
    src/cli/main.cpp
)

target_link_libraries(bmcli
    PRIVATE
        budget_lib
)


# Tests
enable_testing()

add_executable(bmcli_test
    tests/budgetCmds_test.cpp
    tests/categoryCmds_test.cpp
    tests/transactionCmds_test.cpp
    tests/edgeCases_test.cpp
    tests/lifecycleEdgeCases_test.cpp
)

target_link_libraries(bmcli_test
    PRIVATE
        budget_lib
        GTest::gtest_main
)


include(GoogleTest)

gtest_discover_tests(bmcli_test)
