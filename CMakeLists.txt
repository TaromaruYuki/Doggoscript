cmake_minimum_required(VERSION 3.26)
project(cpptest VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
configure_file(DoggoscriptConfig.hpp.in includes/config/DoggoscriptConfig.h)

add_executable(${PROJECT_NAME} src/main.cpp
        src/doggoscript/lexer.cpp
        src/doggoscript/parser.cpp
        src/doggoscript/doggoscript.cpp
        src/doggoscript/interpreter.cpp
        src/doggoscript/token.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR}/includes)
