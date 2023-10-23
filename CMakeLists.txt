cmake_minimum_required(VERSION 3.22)
project(Doggoscript_CLI VERSION 0.4.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} src/main.cpp res/logo/doggoscript.rc)

add_subdirectory(doggoscript)
target_include_directories(${PROJECT_NAME} PRIVATE doggoscript/include)
target_link_libraries(${PROJECT_NAME} PRIVATE doggoscript)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(extern/googletest)

enable_testing()

add_executable(
    Doggoscript_Tests
    tests/lexer_tests.cpp
    tests/parser_tests.cpp
    tests/interpreter_tests.cpp
)

target_include_directories(Doggoscript_Tests PRIVATE doggoscript/include)
target_link_libraries(
    Doggoscript_Tests
    GTest::gtest_main
    doggoscript
)

include(GoogleTest)
gtest_discover_tests(Doggoscript_Tests)