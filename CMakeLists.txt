cmake_minimum_required(VERSION 3.22)
project(Doggoscript VERSION 0.3.0)

set("IS_DEBUG" 0)

if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set("IS_DEBUG" 1)
endif ()

message("Debug mode: ${IS_DEBUG}")

include(CheckIncludeFileCXX)
check_include_file_cxx("format" HAVE_FORMAT)

set(CMAKE_CXX_STANDARD 20)
configure_file(DoggoscriptConfig.hpp.in includes/config/DoggoscriptConfig.h)

add_executable(${PROJECT_NAME} src/main.cpp
        src/doggoscript/lexer.cpp
        src/doggoscript/parser.cpp
        src/doggoscript/doggoscript.cpp
        src/doggoscript/interpreter.cpp
        src/doggoscript/token.cpp
        src/doggoscript/types/classes/number_class.cpp
        src/doggoscript/types/classes/string_class.cpp
        src/doggoscript/types/classes/list_class.cpp
        src/doggoscript/types/classes/dict_class.cpp
        src/doggoscript/types/classes/boolean_class.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR}/includes)
