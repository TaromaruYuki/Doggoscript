cmake_minimum_required(VERSION 3.26)
project(cpptest)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} src/main.cpp
        src/doggoscript/lexer.cpp
        src/doggoscript/parser.cpp
        src/doggoscript/nodes.cpp
        src/doggoscript/doggoscript.cpp
        src/doggoscript/interpreter.cpp
        src/doggoscript/token.cpp)
