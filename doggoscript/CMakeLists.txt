cmake_minimum_required(VERSION 3.22)
project(doggoscript VERSION 0.4.0 DESCRIPTION "A programming language created in C++")

set("IS_DEBUG" 0)

if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" OR CMAKE_BUILD_TYPE STREQUAL "Debug")
    set("IS_DEBUG" 1)
endif ()

message("Debug mode: ${IS_DEBUG}")

include(CheckIncludeFileCXX)
check_include_file_cxx("format" HAVE_FORMAT)

set(CMAKE_CXX_STANDARD 20)
configure_file(DoggoscriptConfig.hpp.in include/config/DoggoscriptConfig.h)

set(DS_SOURCES
    src/lexer.cpp
    src/parser.cpp
    src/doggoscript.cpp
    src/interpreter.cpp
    src/token.cpp
    src/types/classes/number_class.cpp
    src/types/classes/string_class.cpp
    src/types/classes/list_class.cpp
    src/types/classes/dict_class.cpp
    src/types/classes/boolean_class.cpp
        src/types/classes/error_class.cpp
)

add_library(${PROJECT_NAME} STATIC ${DS_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/doggoscript/doggoscript.hpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC include)
